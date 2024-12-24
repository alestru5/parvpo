#include <grpcpp/grpcpp.h>
#include <pqxx/pqxx>
#include "../proto/order_service.grpc.pb.h"
#include "../proto/inventory_service.grpc.pb.h"
#include "../proto/notification_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;
using grpc::Channel;
using grpc::CreateChannel;
using order::OrderService;
using order::OrderRequest;
using order::OrderResponse;
using inventory::InventoryService;
using inventory::InventoryRequest;
using inventory::InventoryResponse;
using notification::NotificationService;
using notification::NotificationRequest;
using notification::NotificationResponse;

class OrderServiceImpl final : public OrderService::Service {
  Status CreateOrder(ServerContext* context, const OrderRequest* request, OrderResponse* reply) override {
    try {
      pqxx::connection C("dbname=orders_db user=user password=password hostaddr=db port=5432");
      if (C.is_open()) {
        pqxx::work W(C);
        W.exec_params("INSERT INTO orders (user_id, product_id, quantity, status) VALUES ($1, $2, $3, 'pending')",
                      request->user_id(), request->product_id(), request->quantity());
        W.commit();
      } else {
        std::cout << "Can't open database" << std::endl;
      }
      C.disconnect ();
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
    InventoryRequest inventory_request;
    inventory_request.set_product_id(request->product_id());
    inventory_request.set_quantity(request->quantity());

    ClientContext inventory_context;
    InventoryResponse inventory_response;
    Status status = inventory_stub_->UpdateInventory(&inventory_context, inventory_request, &inventory_response);

    if (!status.ok()) {
      std::cerr << "Failed to update inventory: " << status.error_message() << std::endl;
    }

    NotificationRequest notification_request;
    notification_request.set_user_id(request->user_id());
    notification_request.set_message("Your order has been processed.");

    ClientContext notification_context;
    NotificationResponse notification_response;
    status = notification_stub_->SendNotification(&notification_context, notification_request, &notification_response);

    if (!status.ok()) {
      std::cerr << "Failed to send notification: " << status.error_message() << std::endl;
    }
    reply->set_message("Order created successfully");
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:5000");
  OrderServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
