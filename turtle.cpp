/*
 * CS475/CS675 - Computer Graphics
 *  ToyLOGO Assignment Base Code
 *
 * Copyright 2009-2014, Parag Chaudhuri, Department of CSE, IIT Bombay
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <cmath>
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>

#include "turtle.hpp"

#define PI 3.14159265

void turtle_t::reset(void) 
{ 
	set_pos(0,0);
	set_dir(0);
}

void turtle_t::clear(void)
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void turtle_t::set_pos(const vertex_t _pos)
{ 	
	pos = _pos;
}

void turtle_t::set_pos(const double _x, const double _y)
{ 
	pos.x = _x;
	pos.y = _y;
}

void turtle_t::set_dir(const double _dir)
{
	dir = _dir;
}

void turtle_t::set_col(const color_t _col)
{
	col = _col;
}

void turtle_t::set_col(const double _r, const double _g, const double _b)
{ 
	col.r = _r;
	col.g = _g;
	col.b = _b;
}

void turtle_t::set_bgcol(const double _r, const double _g, const double _b)
{ 
	glClearColor(_r, _g, _b, 1.0f);
}

void turtle_t::scale(const double _s)
{ 
  glLoadIdentity (); 
  glScalef (1/_s, 1/_s, 1);
}
void turtle_t::pause(const int _t)
{

}

void turtle_t::turn_left(const double _angle)    
{ 
	set_dir(dir+_angle);
}

void turtle_t::turn_right(const double _angle)
{
	set_dir(dir-_angle);
}

void turtle_t::forward(const double _dist)  
{
	glColor4f(col.r, col.g, col.b, 1.0);
	glBegin(GL_LINES);
	glVertex3f(pos.x, pos.y, 0.0f);
	glVertex3f(pos.x + _dist*cos(dir*PI/180), pos.y + _dist*sin(dir*PI/180), 0.0f);
	glEnd();
	set_pos(pos.x + _dist*cos(dir*PI/180), pos.y + _dist*sin(dir*PI/180));
}

void turtle_t::back(const double _dist)   
{
	forward(-1*_dist);
}

void turtle_t::forward_move(const double _dist)
{
	set_pos(pos.x + _dist*cos(dir*PI/180), pos.y + _dist*sin(dir*PI/180));
}

void turtle_t::backward_move(const double _dist)
{ 
	forward_move(-1*_dist);
}

void turtle_t::repeat(const unsigned int &_n, const turtle_com_list_t &_replist)
{ 
  for (int j=0; j < _n; j++)
  {
    for (int i=0; i < _replist.size(); i++)
    {
      exec(_replist[i]);
    }
  }

}



void turtle_t::exec(turtle_com_t *com)
{
  if (com->cname==F)
    {
      turtle_fwd_t* fcom = dynamic_cast<turtle_fwd_t*>(com);
      if (fcom) forward(fcom->dist);
    }
  else if (com->cname==B)
    {
      turtle_bck_t* bcom = dynamic_cast<turtle_bck_t*>(com);
      if (bcom) back(bcom->dist);
    }
  else if (com->cname==L)
    {
      turtle_lft_t* lcom = dynamic_cast<turtle_lft_t*>(com);
      if (lcom) turn_left(lcom->angl);
    }
  else if (com->cname==R)
    {
      turtle_rht_t* rcom = dynamic_cast<turtle_rht_t*>(com);
      if (rcom) turn_right(rcom->angl);
    }
  else if (com->cname==MF)
    {
      turtle_mfwd_t* mfcom = dynamic_cast<turtle_mfwd_t*>(com);
      if (mfcom) forward_move(mfcom->dist);
    }
  else if (com->cname==MB)
    {
      turtle_mbck_t* mbcom = dynamic_cast<turtle_mbck_t*>(com);
      if (mbcom) backward_move(mbcom->dist);
    }
  else if (com->cname==CLS)
    {
      turtle_cls_t* clscom = dynamic_cast<turtle_cls_t*>(com);
      if (clscom) clear();
    }
  else if (com->cname==RESET)
    {
      turtle_rst_t* rstcom = dynamic_cast<turtle_rst_t*>(com);
      if (rstcom) reset();
    }
  else if (com->cname==COL)
    {
      turtle_col_t* colcom = dynamic_cast<turtle_col_t*>(com);
      if (colcom) set_col(colcom->r, colcom->g, colcom->b);
    }
  else if (com->cname==BGCOL)
    {
      turtle_bgcol_t* bgcolcom = dynamic_cast<turtle_bgcol_t*>(com);
      if (bgcolcom) set_bgcol(bgcolcom->r, bgcolcom->g, bgcolcom->b);
    }
  else if (com->cname==SCALE)
    {
      turtle_scale_t* scalecom = dynamic_cast<turtle_scale_t*>(com);
      if (scalecom) scale(scalecom->s);
    }
  else if (com->cname==PAUSE)
    {
      turtle_pause_t* pausecom = dynamic_cast<turtle_pause_t*>(com);
      if (pausecom) pause(pausecom->t);
    }
  else if (com->cname==REPEAT)
    {
      turtle_rep_t *repcom = dynamic_cast<turtle_rep_t*>(com);

      if (repcom)
	{
	  unsigned int times = repcom->times;
	  turtle_com_list_t sublist = repcom->replist;
	  repeat(times, sublist);
	}
     }
  else if ((com->cname==ENDREP) || (com->cname==END) || (com->cname==BEGIN)) 
    {
      //These commands are place holders and used for program structure
      //But no execution is necessary - generate a NoOP
      ;
    }
  else
    {
      std::cerr<<"Unknown Command: Ignoring"<<std::endl;
      exit(-1);
    }
}

void turtle_t::printAttr()
{
	std::cerr<<"Pos: "<<pos.x<<", "<<pos.y<<". Dir: "<<dir<<". Color: "<<col.r<<", "<<col.g<<", "<<col.b<<".\n";
}

