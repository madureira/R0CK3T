# R0CK3T
An ultra-fast C++ micro web framework

## How to use

```c++
#include "R0CK3T.h"
#include <iostream>
#include <string>

using namespace R0CK3T;

class ProductController {
public:
	static void index(const HttpRequest& request)
	{
		std::cout << ">> List of products <<" << std::endl;
	}

	static void getById(const HttpRequest& request)
	{
		std::cout << "Id: " << request.urlParams.at("productId") << std::endl;
	}
};

int main()
{
	Server server("127.0.0.1", "8080", "C:/Users/rafael/projects/workspace/emscripten-test");

	auto app = server.app();

	app->route("/products", &ProductController::index);
	app->route("/products/:productId", &ProductController::getById);

	server.run();

	return 0;
}
```
