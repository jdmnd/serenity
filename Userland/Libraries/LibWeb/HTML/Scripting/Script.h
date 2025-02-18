/*
 * Copyright (c) 2021, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/RefCounted.h>
#include <AK/URL.h>

namespace Web::HTML {

// https://html.spec.whatwg.org/multipage/webappapis.html#concept-script
class Script : public RefCounted<Script> {
public:
    virtual ~Script();

    URL const& base_url() const { return m_base_url; }

protected:
    explicit Script(URL base_url);

private:
    URL m_base_url;
};

}
