# R0CK3T :rocket:
An ultra-fast C++ micro web framework/server

![stability-wip](https://img.shields.io/badge/stability-work_in_progress-lightgrey.svg)

## Basic usage

### config.json
```json
{
  "address": "127.0.0.1",
  "port": "8080"
}
```

### home_controller.h
```c++
#include "R0CK3T.h"

using namespace R0CK3T;

class HomeController {
public:
	static void index(const HttpRequest& request, HttpResponse& response)
	{
		json data;
		data["message"] = "Hello World";

		response.json(data);
	}
};
```

### main.cpp
```c++
#include "R0CK3T.h"
#include "home_controller.h"

using namespace R0CK3T;

int main()
{
	Server server;

	auto app = server.app();

	app->get("/", &HomeController::index);

	server.run();

	return 0;
}
```

### output
```json
{
  "message": "Hello World"
}
```

## Documentation
- [Example](https://github.com/madureira/R0CK3T/blob/master/docs/project_example.md)
- Rest
- Templates

## Libraries

* [mstch](https://github.com/no1msd/mstch)
* [nlohmann/json](https://github.com/nlohmann/json)

## Requirements

* C++11
* [Boost](http://www.boost.org/) (1.65.1)

## Runnig Unit Tests

```sh
$ /R0CK3T/bin/tests/R0CK3T --log_level=test_suite
```
