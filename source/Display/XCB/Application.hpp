//
//  Application.hpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Samuel Williams on 25/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Display/Application.hpp>

namespace Display
{
	namespace XCB
	{
		class Application : public Display::Application
		{
		public:
			Application();
			virtual ~Application();
			
			virtual void run();
			virtual void stop();
			
		protected:
			virtual void setup();
		};
	}
}
