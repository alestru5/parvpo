#include <grpcpp/grpcpp.h>
#include <pqxx/pqxx>
#include "../proto/notification_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using notification::NotificationService;
using notification::NotificationRequest;
using notification::NotificationResponse;

class NotificationServiceImpl final : public NotificationService::Service {
  Status SendNotification(ServerContext* context, const NotificationRequest* request, NotificationResponse* reply) override {
    try {
      pqxx::connection C("dbname=orders_db user=user password=password hostaddr=db port=5432");
      if (C.is_open()) {
        pqxx::work W(C);
        W.exec_params("INSERT INTO notifications (user_id, message) VALUES ($1, $2)",
                      request->user_id(), request->message());
        W.commit();
      } else {
        std::cout << "Can't open database" << std::endl;
      }
      C.disconnect ();
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

    reply->set_message("Notification sent successfully");
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:5002");
  NotificationServiceImpl service;

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
