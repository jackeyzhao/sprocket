// Copyright (c) 2015 University of Szeged.
// Copyright (c) 2015 The Chromium Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SPROCKET_BROWSER_BROWSER_MAIN_PARTS_H_
#define SPROCKET_BROWSER_BROWSER_MAIN_PARTS_H_

#include "content/public/browser/browser_main_parts.h"
#include "content/public/common/main_function_params.h"
#include "sprocket/browser/browser_context.h"

namespace devtools_http_handler {
class DevToolsHttpHandler;
}

// This class contains different "stages" to be executed by |BrowserMain()|,
// Each stage is represented by a single BrowserMainParts method, called from
// the corresponding method in |BrowserMainLoop| (e.g., EarlyInitialization())
// which does the following:
//  - calls a method (e.g., "PreEarlyInitialization()") which implements
//    platform / tookit specific code for that stage.
//  - calls various methods for things common to all platforms (for that stage).
//  - calls a method (e.g., "PostEarlyInitialization()") for platform-specific
//    code to be called after the common code.
//
// Stages:
//  - EarlyInitialization: things which should be done as soon as possible on
//    program start (such as setting up signal handlers) and things to be done
//    at some generic time before the start of the main message loop.
//  - MainMessageLoopStart: things beginning with the start of the main message
//    loop and ending with initialization of the main thread; platform-specific
//    things which should be done immediately before the start of the main
//    message loop should go in |PreMainMessageLoopStart()|.
//  - RunMainMessageLoopParts:  things to be done before and after invoking the
//    main message loop run method (e.g. MessageLoopForUI::current()->Run()).

class SprocketBrowserMainParts : public content::BrowserMainParts {
 public:
  explicit SprocketBrowserMainParts(const content::MainFunctionParams& parameters);
  ~SprocketBrowserMainParts() override;

  SprocketBrowserContext* browser_context() {
    return browser_context_.get();
  }
  SprocketBrowserContext* off_the_record_browser_context() {
    return off_the_record_browser_context_.get();
  }

  // content::BrowserMainParts overrides.

  void PreEarlyInitialization() override;

  void PostMainMessageLoopStart() override;

  // This is called just before the main message loop is run.  The
  // various browser threads have all been created at this point.
  void PreMainMessageLoopRun() override;

  // This happens after the main message loop has stopped, but before
  // threads are stopped.
  void PostMainMessageLoopRun() override;

  devtools_http_handler::DevToolsHttpHandler* devtools_http_handler() {
    return devtools_http_handler_.get();
  }

 protected:
  virtual void InitializeBrowserContexts();
  virtual void InitializeMessageLoopContext();

  void set_browser_context(SprocketBrowserContext* context) {
    browser_context_.reset(context);
  }
  void set_off_the_record_browser_context(SprocketBrowserContext* context) {
    off_the_record_browser_context_.reset(context);
  }

 private:
  std::unique_ptr<SprocketBrowserContext> browser_context_;
  std::unique_ptr<SprocketBrowserContext> off_the_record_browser_context_;
  const content::MainFunctionParams parameters_;
  std::unique_ptr<devtools_http_handler::DevToolsHttpHandler> devtools_http_handler_;

  DISALLOW_COPY_AND_ASSIGN(SprocketBrowserMainParts);
};

#endif // SPROCKET_BROWSER_BROWSER_MAIN_PARTS_H_
