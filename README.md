# R0CK3T :rocket:
An ultra-fast C++ micro web framework

![stability-wip](https://img.shields.io/badge/stability-work_in_progress-lightgrey.svg)

## How to use

```c++
// file: product_controller.h

#include "R0CK3T.h"
#include <iostream>

using namespace R0CK3T;

class ProductController {
public:
	static void index(const HttpRequest& request, HttpResponse& response)
	{
		std::cout << ">> List of products <<" << std::endl;
	}

	static void getById(const HttpRequest& request, HttpResponse& response)
	{
		std::cout << "Id: " << request.urlParams.at("productId") << std::endl;
	}
};
```
```c++
// file: main.cpp

#include "R0CK3T.h"
#include "product_controller.h"

using namespace R0CK3T;

int main()
{
	Server server("127.0.0.1", 8080, "/absolute/path/to/assets");

	auto app = server.app();

	app->route("/products", &ProductController::index);
	app->route("/products/:productId", &ProductController::getById);

	server.run();

	return 0;
}
```

## Requirements

* C++11
* [Boost](http://www.boost.org/) (1.65.1)

## Runnig Unit Tests

```sh
$ /R0CK3T/bin/tests/R0CK3T --log_level=test_suite
```
