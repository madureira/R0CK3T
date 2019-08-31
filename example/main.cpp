#include "../include/R0CK3T.h"
#include "controllers/home_controller.h"
#include "controllers/product_controller.h"

using namespace R0CK3T;
using namespace example;

int main()
{
	Server server;

	auto app = server.app();

	app->get("/", &HomeController::index);
	app->get("/products", &ProductController::index);
	app->get("/products/:productId", &ProductController::getById);

	server.run();

	return 0;
}