#pragma once

#include <iostream>

namespace example {

	class HomeController {
	public:
		static void index()
		{
			std::cout << ">> Home template <<" << std::endl;
		}
	};

}