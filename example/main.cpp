#include "../include/R0CK3T.h"
#include "controllers/home_controller.h"
#include "controllers/product_controller.h"

using namespace R0CK3T;
using namespace example;

int main()
{
	Server server("127.0.0.1", 8080, "C:/Users/rafael/projects/workspace/emscripten-test");

	auto app = server.app();

	app->route("/", &HomeController::index);
	app->route("/products", &ProductController::index);
	app->route("/products/:productId", &ProductController::getById);
	app->route("/products/name/:productName", &ProductController::getByName);

	server.run();

	return 0;
}