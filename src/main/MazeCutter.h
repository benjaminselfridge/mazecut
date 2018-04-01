/*
 *  Copyright 2010 Ben Selfridge
 *
 *  This file is part of mazecut.
 *
 *  mazecut is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mazecut is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with mazecut.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * MazeCutter.h
 */

#ifndef MAZECUTTER_H_
#define MAZECUTTER_H_

#include <string>
#include <wx/menu.h>
#include "Maze2d.h"

namespace MazeCutterVariables {

const int CLUTTERHEIGHT = 25;
const int STARTWIDTH = 555;
const int STARTHEIGHT = 555 + CLUTTERHEIGHT;
const int DEFAULTNUMROWS = 10;
const int DEFAULTNUMCOLS = 10;
const bool MARGINPROPORTIONAL = false;
const int DEFAULTMARGINDIVISOR = 20;
const int DEFAULTMARGINSIZE = 20;
const int BORDERTHICKNESSDIVISOR = 10;
const std::string DEFAULTALGORITHM = "rb";
bool SQUARENODES = true;

enum widget_ids {
	ID_RBBUTTON = 101,
	ID_PBUTTON,
	ID_EBUTTON,
	ID_BTBUTTON,
	ID_ABBUTTON,
	ID_WBUTTON,
	ID_SBUTTON,
	ID_ROWENTRY,
	ID_COLUMNENTRY,
	ID_CREATEBUTTON,
	ID_CLOSEBUTTON,
	ID_IMAGEEXPORT
};

}

class BasicDrawPane : public wxPanel
{

	int current_width, current_height;

public:
	Maze2d *maze;
	BasicDrawPane(wxFrame* parent, Maze2d *maze);

	void OnPaint(wxPaintEvent & evt);
	// void paintNow();

	void renderMaze(Maze2d *maze, wxDC& dc);
	void renderMazeNode(MazeNode& node, int rows, int cols, int x_base, int y_base, int nodeWidth, int nodeHeight, wxDC& dc);

	void repaint();

	void OnResize(wxSizeEvent& sizeEvent);

	// some useful events
	/*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
	 */

	DECLARE_EVENT_TABLE()

};


class MazeCutter : public wxFrame {

	wxMenuBar *menubar;
	wxMenu *file;
	wxMenuItem *newMaze;
	wxMenuItem *imageExport;
	wxMenuItem *open;
	wxMenuItem *quit;

	Maze2d *maze;
	std::string defaultAlgorithm;
	int defaultRows, defaultCols;

	BasicDrawPane *drawPane;

public:
	MazeCutter(const wxString& title);

	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnImageExport(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

};


class MazeCutterApp : public wxApp
{
public:
	virtual bool OnInit();
};

class NewMazeDialog : public wxDialog
{
	wxRadioButton *rb;
	wxRadioButton *p;
	wxRadioButton *e;
	wxRadioButton *bt;
	wxRadioButton *ab;
	wxRadioButton *w;
	wxRadioButton *s;

	wxStaticText *rowLabel;
	wxTextCtrl *rowEntry;
	wxStaticText *columnLabel;
	wxTextCtrl *columnEntry;

	bool canceled;
	std::string algorithm;
	int rows;
	int cols;

	Maze2d *maze;

public:
	NewMazeDialog(const wxString& title, std::string defaultAlgorithm, int defaultRows, int defaultCols);

	void OnCreate(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);

	void OnSetRB(wxCommandEvent& event);
	void OnSetP(wxCommandEvent& event);
	void OnSetE(wxCommandEvent& event);
	void OnSetBT(wxCommandEvent& event);
	void OnSetAB(wxCommandEvent& event);
	void OnSetW(wxCommandEvent& event);
	void OnSetS(wxCommandEvent& event);

	Maze2d *getMaze() { return maze; }
	std::string getAlgorithm() { return algorithm; }
	int getRows() { return rows; }
	int getCols() { return cols; }
	bool isCanceled() { return canceled; }
};



#endif /* MAZECUTTER_H_ */
