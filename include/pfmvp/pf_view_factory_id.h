// Copyright (c) 2015
// All rights reserved.
//
// Author: Lutts Cao <<lutts.cao@gmail.com>>
//
// [Desc]

#ifndef INCLUDE_PFMVP_PF_VIEW_FACTORY_ID_H_
#define INCLUDE_PFMVP_PF_VIEW_FACTORY_ID_H_

#define INVALID_PF_VIEW_FACTORY_ID \
  utils::U8String { "" }
#define isViewFactoryIdValid(id) (id != INVALID_PF_VIEW_FACTORY_ID)

#define PF_VIEW_FACTORY(factory) (#factory)

#endif  // INCLUDE_PFMVP_PF_VIEW_FACTORY_ID_H_
