/**
 * (C) 2010-2011 Alibaba Group Holding Limited.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 * 
 * Version: $Id$
 *
 * ob_regex.cpp for ...
 *
 * Authors:
 *   yubai <yubai.lk@taobao.com>
 *
 */
#include <stdlib.h>
#include "tblog.h"
#include "ob_malloc.h"
#include "ob_regex.h"

namespace oceanbase
{
  namespace common
  {
    ObRegex::ObRegex()
    {
      init_ = false;
      match_ = NULL;
      nmatch_ = 0;
    }
    
    ObRegex::~ObRegex()
    {
      if (true == init_)
      {
        destroy();
      }
    }
    
    bool ObRegex::init(const char* pattern, int flags)
    {
      bool bret = false;
      if (init_) 
      {
        TBSYS_LOG(WARN, "this=%p already inited", this);
      }
      else if (NULL == pattern)
      {
        TBSYS_LOG(WARN, "invalid param pattern=%p", pattern);
      }
      else
      {
        int tmp_ret = regcomp(&reg_, pattern, flags);
        if (0 != tmp_ret)
        {
          TBSYS_LOG(WARN, "regcomp fail ret=%d", tmp_ret);
        }
        else
        {
          nmatch_ = reg_.re_nsub + 1;
          match_ = (regmatch_t*)ob_malloc(sizeof(regmatch_t) * nmatch_);
          if (NULL == match_)
          {
            TBSYS_LOG(WARN, "create the regmatch object fail");
            regfree(&reg_);
          }
          else
          {
            init_ = true;
            bret = true;
          }
        }
      }
      return bret;
    }
    
    bool ObRegex::match(const char* text, int flags)
    {
      bool bret = false;
      if (!init_)
      {
        TBSYS_LOG(WARN, "this=%p have not inited", this);
      }
      else if (NULL == text)
      {
        TBSYS_LOG(WARN, "invalid param text=%p", text);
      }
      else
      {
        int tmp_ret = regexec(&reg_, text, nmatch_, match_, flags);
        if (REG_NOMATCH == tmp_ret)
        {
          bret = false;
        }
        else if (tmp_ret != 0)
        {
          bret = false;
        }
        else
        {
          bret = true;
        }
      }
      return bret;
    }
    
    void ObRegex::destroy(void)
    {
      if (init_)
      {
        regfree(&reg_);
        if (NULL != match_)
        {
          ob_free(match_);
          match_ = NULL;
        }
        nmatch_ = 0;
        init_ = false;
      }
    }
  }
}


