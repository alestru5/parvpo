#include <grpcpp/grpcpp.h>
#include <pqxx/pqxx>
#include "../proto/inventory_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using inventory::InventoryService;
using inventory::InventoryRequest;
using inventory::InventoryResponse;

class InventoryServiceImpl final : public InventoryService::Service {
  Status UpdateInventory(ServerContext* context, const InventoryRequest* request, InventoryResponse* reply) override {
    try {
      pqxx::connection C("dbname=orders_db user=user password=password hostaddr=db port=5432");
      if (C.is_open()) {
        pqxx::work W(C);
        W.exec_params("UPDATE inventory SET quantity = quantity - $1 WHERE product_id = $2",
                      request->quantity(), request->product_id());
        W.commit();
      } else {
        std::cout << "Can't open database" << std::endl;
      }
      C.disconnect ();
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

    reply->set_message("Inventory updated successfully");
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:5001");
  InventoryServiceImpl service;

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
