# Documentation
This doc will guide you to build your own application using R0CK3T through a project example.

## Prerequisites
- C++11
- [Boost](http://www.boost.org/) (1.65.1)

## Usage

### 1. Install prerequisites

### 2. Generate project structure
The `project structure`, should look like:

```
project/
├── config.json
├── main.cpp
├── controllers/
│   ├── home_controller.h
│   └── api_controller.h
├── templates/
│   ├── index.tmpl
│   ├── header.tmpl
│   └── footer.tmpl
└── include/
│   ├── R0CK3T.h
│   └── R0CK3T/
│       ├── renderer/
│       ├── vendors/
│       ├── app.cpp
│       ├── app.h
│       └── ...
└── bin/
```

#### config.json
```json
{
  "address": "127.0.0.1",
  "port": "8080",
  "templatesPath": "/absolute/path/to/templates"
}
```

#### templates/header.tmpl
```html
<html>
<head>
  <title>Template Test</title>
</head>
<body>
```

#### templates/index.tmpl
```html
{{> header}}
<h1>{{ title }}</h1>
{{> footer}}
```

#### templates/footer.tmpl
```html
</body>
</html>
```

#### controllers/home_controller.h
```c++
#include "../include/R0CK3T.h"

using namespace R0CK3T;

class HomeController {
public:
    static void index(const HttpRequest& request, HttpResponse& response)
    {
        mstch::map data { { "title", std::string{ "Hello World" } } };

        std::map<std::string, std::string> partials = {{ "header", "header.tmpl" },{ "footer", "footer.tmpl" }};

        Template::instance().render("index.tmpl", data, partials, response);
    }
};
```

#### controllers/api_controller.h
```c++
#include "../include/R0CK3T.h"

using namespace R0CK3T;

class ApiController {
public:
    static void productsList(const HttpRequest& request, HttpResponse& response)
    {
        json data = {{{"id", 123}, {"name", "C++ Book"}}, {{"id", 456}, {"name", "Java Book"}}};

        JSON::render(data, response);
    }

    static void productById(const HttpRequest& request, HttpResponse& response)
    {
        json data;
        data["id"] = request.getUrlParam("id");
        data["name"] = "C++ Book";
        data["price"]["from"] = 60.00;
        data["price"]["to"] = 39.99;

        JSON::render(data, response);
    }
};
```

#### main.cpp
```c++
#include "include/R0CK3T.h"
#include "controllers/home_controller.h"
#include "controllers/api_controller.h"

using namespace R0CK3T;

int main()
{
  Server server;

  auto app = server.app();

  app->get("/", &HomeController::index);
  app->get("/products", &ApiController::productsList);
  app->get("/products/:id", &ApiController::productById);

  server.run();

  return 0;
}
```

### 3. Build project

### 4. Start the web server
```sh
$ project/bin/app start
```

### 5. Acess the resources
```
http://localhost:8080/
```

```html
<html>
<head>
  <title>Template Test</title>
</head>
<body>
  <h1>Hello World</h1>
</body>
</html>
```

```
http://localhost:8080/products
```

```json
[
  {
    "id": 123,
    "name": "C++ Book"
  },
  {
    "id": 456,
    "name": "Java Book"
  }
]
```

```
http://localhost:8080/products/123
```

```json
{
  "id": 123,
  "name": "C++ Book",
  "price": {
    "from": 60,
    "to": 39.99
  }
}
```

## Author

[Rafael Madureira](https://github.com/madureira)
