//
//  Application.cpp
//  This file is part of the "Vizor Platform XCB" project and released under the .
//
//  Created by Huba Nagy on 1/3/2019.
//  Copyright, 2019, by Huba Nagy. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Display/XCB/Application.hpp>

namespace Display
{
	namespace XCB
	{
		UnitTest::Suite ApplicationTestSuite {
			"Display::XCB::Application",
			
			{"it should have some real tests",
				[](UnitTest::Examiner & examiner) {
					examiner.expect(false).to(be_true);
				}
			},
		};
	}
}
