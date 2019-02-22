//
//  Application.cpp
//  This file is part of the "Vizor Platform" project and released under the .
//
//  Created by Samuel Williams on 16/2/2019.
//  Copyright, 2019, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Vizor/Platform/Cocoa/Application.hpp>

namespace Vizor
{
	namespace Platform
	{
		namespace Cocoa
		{
			class TestApplication : public Application
			{
			public:
				using Application::Application;
				virtual ~TestApplication() {}
				
				virtual void did_finish_launching()
				{
					this->stop();
				}
			};
			
			UnitTest::Suite ApplicationTestSuite {
				"Vizor::Platform::Cocoa::Application",
				
				{"it can start application",
					[](UnitTest::Examiner & examiner) {
						TestApplication application;
						
						application.run();
					}
				},
			};
		}
	}
}