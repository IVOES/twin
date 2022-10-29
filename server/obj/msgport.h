/*
 *  msg.h  --  declare server class Smsg
 *
 *  Copyright (C) 1993-2019 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#ifndef TWIN_MSGPORT_H
#define TWIN_MSGPORT_H

#include "obj/fwd.h"
#include "obj/window.h" // struct s_remotedata
#include <Tw/datatypes.h>

struct SmsgportFn {
  uldat Magic;
  void (*Insert)(msgport, all, msgport Prev, msgport Next);
  void (*Remove)(msgport);
  void (*Delete)(msgport);
  void (*ChangeField)(msgport, udat field, uldat CLEARMask, uldat XORMask);
  /* msgport */
  TobjFn Fn_Obj;
};

struct Smsgport : public Sobj {
  TmsgportFn Fn;
  msgport Prev, Next; /* list in the same All */
  all All;
  /* msgport */
  byte WakeUp, NameLen;
  char *Name;
  /* Note : a MsgPort is always woken up if it has pending messages. */
  void (*Handler)(msgport);
  void (*ShutDownHook)(msgport);
  timevalue CallTime, PauseDuration;
  remotedata RemoteData;
  msg FirstMsg, LastMsg;
  menu FirstMenu, LastMenu;     /* menus created by this MsgPort */
  Twidget FirstW, LastW;        /* widgets owned by this MsgPort */
  ggroup FirstGroup, LastGroup; /* groups owned by this MsgPort */
  mutex FirstMutex, LastMutex;  /* mutexes owned by this MsgPort */
  Tdisplay AttachHW;            /* that was attached as told by MsgPort */

  static msgport Create(byte NameLen, const char *Name, tany PauseSec, tany PauseFraction,
                        byte WakeUp, void (*Handler)(msgport));
  msgport Init(byte NameLen, const char *Name, tany PauseSec, tany PauseFraction, byte WakeUp,
               void (*Handler)(msgport));

  /* obj */
  uldat Magic() const {
    return Fn->Magic;
  }
  void Insert(all a, msgport prev, msgport next) {
    Fn->Insert(this, a, prev, next);
  }
  void Remove() {
    Fn->Remove(this);
  }
  void Delete() {
    Fn->Delete(this);
  }
  /* msgport */
};

/* MsgPort->WakeUp: */
#define TIMER_ALWAYS ((byte)1)
#define TIMER_ONCE ((byte)2)

#endif /* TWIN_MSGPORT_H */
