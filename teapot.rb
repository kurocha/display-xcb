# Teapot v3.0.0 configuration generated at 2019-02-22 17:20:05 +1300

required_version "3.0"

define_project "display-xcb" do |project|
	project.title = "Vizor Platform XCB"
end

# Build Targets

define_target 'display-xcb-platform' do |target|
	target.provides 'Display/XCB/Platform' do
		append linkflags %W{-lxcb}
	end
end

define_target 'display-xcb-library' do |target|
	target.depends 'Language/C++14'
	
	target.depends 'Display/XCB/Platform', public: true
	
	target.depends 'Library/Logger', public: true
	target.depends 'Library/Display', public: true
	
	target.provides 'Library/Display/XCB' do
		source_root = target.package.path + 'source'
		
		library_path = build static_library: 'Display/XCB', source_files: source_root.glob('Display/XCB/**/*.cpp')
		
		append linkflags library_path
		append header_search_paths source_root
	end
	
	target.provides :display_native => 'Library/Display/XCB'
end

define_target 'display-xcb-test' do |target|
	target.depends 'Library/Display/XCB'
	target.depends 'Library/UnitTest'
	
	target.depends 'Language/C++14'
	
	target.provides 'Test/Display/XCB' do |*arguments|
		test_root = target.package.path + 'test'
		
		run tests: 'Display/XCB', source_files: test_root.glob('Display/XCB/**/*.cpp'), arguments: arguments
	end
end

# Configurations

define_configuration 'development' do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	configuration.import "display-xcb"
	
	# Provides all the build related infrastructure:
	configuration.require 'platforms'
	
	# Provides unit testing infrastructure and generators:
	configuration.require 'unit-test'
	
	# Provides some useful C++ generators:
	configuration.require 'generate-cpp-class'
	
	configuration.require "generate-project"
end

define_configuration "display-xcb" do |configuration|
	configuration.public!
	
	configuration.require "display"
end
