// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#ifndef __PBWAFER_H
#define __PBWAFER_H

#define INIT_NET     1
#define INIT_PROTO   2
#define INIT_VID     4
#define INIT_RENDER  8
#define INIT_INPUT   16
#define INIT_SND     32

void PBWAFER_init(char mask);
void PBWFAER_quit();

#endif /* !__PBWAFER_H */
