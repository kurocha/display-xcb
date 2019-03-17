//
//  Window.cpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Huba Nagy on 22/2/2019.
//  Copyright, 2019, by Huba Nagy. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Display/XCB/Window.hpp>

#include <Time/Interval.hpp>

namespace Display
{
	namespace XCB
	{
		using namespace UnitTest::Expectations;
		
		UnitTest::Suite WindowTestSuite {
			"Display::XCB::Window",
			
			{"it should be able to show a window for a few seconds",
				[](UnitTest::Examiner & examiner) {
					Application application;
					Window window(application);

					window.set_title("doot!");
					window.show();

					application.run();

					window.hide();
				}
			},
		};
	}
}
