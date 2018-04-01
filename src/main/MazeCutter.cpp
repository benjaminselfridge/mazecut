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
 * MazeCutter.cpp
 */

/*
 * TODO: Center maze (adjust margins). It's sort of centered now but make it better
 */

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/dcbuffer.h>
#include "MazeCutter.h"
#include "Maze2d.h"
#include <iostream>
#include <algorithm>

using namespace MazeCutterVariables;

BEGIN_EVENT_TABLE(MazeCutter, wxFrame)
EVT_PAINT(MazeCutter::OnPaint)
// EVT_SIZE(MazeCutter::OnResize)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
// some useful events
/*
 EVT_MOTION(BasicDrawPane::mouseMoved)
 EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
 EVT_LEFT_UP(BasicDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(BasicDrawPane::OnPaint)
// Resizes the maze with the window
EVT_SIZE(BasicDrawPane::OnResize)

END_EVENT_TABLE()

MazeCutter::MazeCutter(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(STARTWIDTH, STARTHEIGHT)) {
	// SetIcon(wxIcon(wxT("$HOME/bin/MazeCutterIcons/maze.jpg")));
	maze = new Maze2d(10, 10, "ellers");
	defaultAlgorithm = DEFAULTALGORITHM;
	defaultRows = DEFAULTNUMROWS;
	defaultCols = DEFAULTNUMCOLS;

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	drawPane = new BasicDrawPane( (wxFrame*) this, maze );
	// drawPane->maze = maze;
	sizer->Add(drawPane, 1, wxEXPAND);

	this->SetSizer(sizer);
	this->SetAutoLayout(true);

	menubar = new wxMenuBar;
	file = new wxMenu;

	// newMenu = new wxMenu;
	// newMenu->Append(wxID_NEW, wxT("Maze"));

	// file->Append(wxID_ANY, wxT("&New"));
	// file->AppendSubMenu(newMenu, wxT("&New"));
	// file->Append(wxID_ANY, wxT("&Open"));
	// file->Append(wxID_SAVE, wxT("&Save as..."));
	newMaze = new wxMenuItem(file, wxID_NEW, wxT("&New\tCtrl+N"));
	file->Append(newMaze);
	open = new wxMenuItem(file, wxID_OPEN, wxT("&Open\tCtrl+O"));
	file->Append(open);
	imageExport = new wxMenuItem(file, ID_IMAGEEXPORT, wxT("&Export to image\tCtrl+E"));
	file->Append(imageExport);
	file->AppendSeparator();

	// file->AppendSubMenu(imp, wxT("I&mport"));

	quit = new wxMenuItem(file, wxID_EXIT, wxT("&Quit\tCtrl+Q"));
	file->Append(quit);

	menubar->Append(file, wxT("&File"));
	SetMenuBar(menubar);

	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(MazeCutter::OnQuit));

	Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(MazeCutter::OnNew));

	Connect(ID_IMAGEEXPORT, wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(MazeCutter::OnImageExport));

	// Connect(wxEVT_PAINT, wxPaintEventHandler(MazeCutter::OnPaint));
	Centre();
}

void MazeCutter::OnPaint(wxPaintEvent& event) {
  //    std::cout << "MazeCutter::OnPaint\n";
  //	std::cout.flush();
	wxBufferedPaintDC dc(this);
	drawPane->maze = maze;
	drawPane->renderMaze(maze, dc);
}

void MazeCutter::OnResize(wxSizeEvent& sizeEvent) {
	// wxClientDC dc(this);
	// drawPane->renderMaze(maze, dc);
}

void MazeCutter::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MazeCutter::OnNew(wxCommandEvent& event) {
	NewMazeDialog *newMazeDialog = new NewMazeDialog(wxT("New Maze"), defaultAlgorithm, defaultRows, defaultCols);
	newMazeDialog->Show(true);

	// newMazeDialog will create the maze.

	// setMaze(new Maze2d(DEFAULTNUMROWS, DEFAULTNUMCOLS, DEFAULTALGORITHM));
	// if (maze != NULL) delete maze;
	maze = newMazeDialog->getMaze();
	drawPane->maze = maze;
	defaultAlgorithm = newMazeDialog->getAlgorithm();
	defaultRows = newMazeDialog->getRows();
	defaultCols = newMazeDialog->getCols();
	Refresh();
}

void MazeCutter::OnImageExport(wxCommandEvent& event) {
    wxFileDialog exportDialog(this, _("Export maze as image"), _(""), _(""),
				  _("BMP (*.bmp)|*.bmp|JPEG (*.jpg)|*.jpg|PNG (*.png)|*.png|TIFF (*.tif)|*.tif"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if (exportDialog.ShowModal() == wxID_CANCEL)
		return;

	wxBitmap bitmap(drawPane->GetSize().x, drawPane->GetSize().y);
	wxMemoryDC memoryDC(bitmap);
	drawPane->maze = maze;
	drawPane->renderMaze(maze, memoryDC);

	wxImage image = bitmap.ConvertToImage();
	wxInitAllImageHandlers();
	image.SaveFile(exportDialog.GetPath());
}

BasicDrawPane::BasicDrawPane(wxFrame* parent, Maze2d *maze) :
		wxPanel(parent)
{
	// SetBackgroundColour(wxColour(1,0,0));
}

void BasicDrawPane::OnPaint(wxPaintEvent & evt) {
	// renderMaze(maze);
	// std::cout << "BasicDrawPane::OnPaint\n";
	// std::cout.flush();
	wxBufferedPaintDC dc(this);
	renderMaze(maze, dc);
	// dc.DrawLine(0, 0, 100, 100);
}

void BasicDrawPane::renderMaze(Maze2d *maze, wxDC &dc) {
	this->maze = maze;
	dc.Clear();
	// dc.DrawLine(0,0,100,100);

	if (this->maze != NULL) {
		int frameWidth, frameHeight;
		this->GetSize(&frameWidth, &frameHeight);

		int fake_horizontalMargin, fake_verticalMargin;
		if (MARGINPROPORTIONAL) {
			fake_horizontalMargin = frameWidth / DEFAULTMARGINDIVISOR;
			fake_verticalMargin = frameHeight / DEFAULTMARGINDIVISOR;
		}
		else {
			fake_horizontalMargin = DEFAULTMARGINSIZE;
			fake_verticalMargin = DEFAULTMARGINSIZE;
		}
		int fake_left = fake_horizontalMargin;
		int fake_right = frameWidth - fake_horizontalMargin;
		int fake_top = fake_verticalMargin;
		int fake_bottom = frameHeight - fake_verticalMargin;

		int nodeWidth = (fake_right - fake_left) / maze->numCols();
		int nodeHeight = (fake_bottom - fake_top) / maze->numRows();
		if (SQUARENODES) {
			if (nodeHeight > nodeWidth) nodeHeight = nodeWidth;
			else if (nodeWidth > nodeHeight) nodeWidth = nodeHeight;
		}
		int wallThickness = nodeWidth < nodeHeight ?
				(nodeWidth / BORDERTHICKNESSDIVISOR) :
				(nodeHeight / BORDERTHICKNESSDIVISOR);
		wallThickness = wallThickness == 0 ? 1 : wallThickness;

		wxPen pen(wxColor(0,0,0), wallThickness);
		pen.SetCap(wxCAP_PROJECTING);
		dc.SetPen(pen);

		if (nodeWidth <=1 || nodeHeight <= 1) return;

		int mazeWidth = nodeWidth * maze->numCols();
		int mazeHeight = nodeHeight * maze->numRows();
		int horizontalMargin = (frameWidth - mazeWidth) / 2;
		int verticalMargin = (frameHeight - mazeHeight) / 2;
		int left = horizontalMargin;
		int top = verticalMargin;

		for (int r=0; r < maze->numRows(); r++) {
			for(int c=0; c < maze->numCols(); c++) {
				int x_base = left + nodeWidth * c;
				int y_base = top + nodeHeight * r;
				renderMazeNode(maze->getCopyOfNode(r, c), maze->numRows(), maze->numCols(), x_base, y_base, nodeWidth, nodeHeight, dc);
			}
		}
	}

}

void BasicDrawPane::renderMazeNode(MazeNode& node, int rows, int cols, int x_base, int y_base, int nodeWidth, int nodeHeight, wxDC& dc) {
	bool isTopLeft = node.row == 0 && node.col == 0;
	bool isBottomRight = node.row == rows-1 && node.col == cols-1;

	if (!node.nodeTo(UP) && !isTopLeft) {
		dc.DrawLine(x_base, y_base, x_base + nodeWidth, y_base);
	}
	if (!node.nodeTo(LEFT)) {
		dc.DrawLine(x_base, y_base, x_base, y_base + nodeHeight);
	}
	if (!node.nodeTo(DOWN) && !isBottomRight) {
		dc.DrawLine(x_base, y_base + nodeHeight, x_base + nodeWidth, y_base + nodeHeight);
	}
	if (!node.nodeTo(RIGHT)) {
		dc.DrawLine(x_base + nodeWidth, y_base, x_base + nodeWidth, y_base + nodeHeight);
	}
}

void BasicDrawPane::repaint() {
	int width, height;
	this->GetSize(&width, &height);
	if (this->current_width != width || this->current_height != height) {
		wxClientDC cdc(this);
		wxBufferedDC dc(&cdc);
		renderMaze(maze, dc);
		this->current_width = width;
		this->current_height = height;
	}
}

void BasicDrawPane::OnResize(wxSizeEvent& sizeEvent) {
	// this->ClearBackground();
	repaint();
}

IMPLEMENT_APP(MazeCutterApp)

bool MazeCutterApp::OnInit()
{

	MazeCutter *mazecutter = new MazeCutter(wxT("MazeCutter"));
	mazecutter->Show(true);

	return true;
}

NewMazeDialog::NewMazeDialog(const wxString & title, std::string defaultAlgorithm, int defaultRows, int defaultCols)
: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 25 * 8 + 120))
{

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	new wxStaticBox(panel, -1, wxT("Algorithm"),
			wxPoint(5, 5), wxSize(240, 25 * 8 + 30));
	rb = new wxRadioButton(panel, ID_RBBUTTON, wxT("Recursive Backtracking"), wxPoint(15, 30));
	p = new wxRadioButton(panel, ID_PBUTTON, wxT("Prim's"), wxPoint(15, 55));
	e = new wxRadioButton(panel, ID_EBUTTON, wxT("Eller's"), wxPoint(15, 80));
	bt = new wxRadioButton(panel, ID_BTBUTTON, wxT("Binary Tree"), wxPoint(15, 105));
	ab = new wxRadioButton(panel, ID_ABBUTTON, wxT("Aldous-Broder"), wxPoint(15, 130));
	w = new wxRadioButton(panel, ID_WBUTTON, wxT("Wilson's"), wxPoint(15, 155));
	s = new wxRadioButton(panel, ID_SBUTTON, wxT("Sidewinder"), wxPoint(15, 180));

	if (defaultAlgorithm == "rb") {
		rb->SetValue(true);
	} else if (defaultAlgorithm == "p") {
		p->SetValue(true);
	} else if (defaultAlgorithm == "e") {
		e->SetValue(true);
	} else if (defaultAlgorithm == "bt") {
		bt->SetValue(true);
	} else if (defaultAlgorithm == "ab") {
		ab->SetValue(true);
	} else if (defaultAlgorithm == "w") {
		w->SetValue(true);
	} else if (defaultAlgorithm == "s") {
		s->SetValue(true);
	} else {
		bt->SetValue(true);
	}
	algorithm = defaultAlgorithm;
	rows = defaultRows;
	cols = defaultCols;
	std::stringstream rowStringStream;
	rowStringStream << rows;
	std::stringstream colStringStream;
	colStringStream << cols;

	wxString rowString = wxString::FromAscii(rowStringStream.str().c_str());
	wxString colString = wxString::FromAscii(colStringStream.str().c_str());

	// std::cout << wxT(rowString) << "\n";

	rowLabel = new wxStaticText(panel, -1, wxT("Rows: "), wxPoint(15, 205));
	rowEntry = new wxTextCtrl(panel, ID_ROWENTRY, rowString, wxPoint(60, 205), wxSize(50, 20));
	columnLabel = new wxStaticText(panel, -1, wxT("Columns: "), wxPoint(120, 205));
	columnEntry = new wxTextCtrl(panel, ID_COLUMNENTRY, colString, wxPoint(188, 205), wxSize(50,20));


	wxButton *createButton = new wxButton(this, ID_CREATEBUTTON, wxT("Create"),
			wxDefaultPosition, wxSize(70, 30));
	wxButton *closeButton = new wxButton(this, ID_CLOSEBUTTON, wxT("Cancel"),
			wxDefaultPosition, wxSize(70, 30));

	createButton->SetFocus();

	Connect(ID_RBBUTTON, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
			wxCommandEventHandler(NewMazeDialog::OnSetRB));
	Connect(ID_PBUTTON, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
			wxCommandEventHandler(NewMazeDialog::OnSetP));
	Connect(ID_EBUTTON, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
			wxCommandEventHandler(NewMazeDialog::OnSetE));
	Connect(ID_BTBUTTON, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
			wxCommandEventHandler(NewMazeDialog::OnSetBT));
	Connect(ID_ABBUTTON, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
			wxCommandEventHandler(NewMazeDialog::OnSetAB));
	Connect(ID_WBUTTON, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
			wxCommandEventHandler(NewMazeDialog::OnSetW));
	Connect(ID_SBUTTON, wxEVT_COMMAND_RADIOBUTTON_SELECTED,
			wxCommandEventHandler(NewMazeDialog::OnSetS));

	Connect(ID_CREATEBUTTON, wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(NewMazeDialog::OnCreate));
	Connect(ID_CLOSEBUTTON, wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(NewMazeDialog::OnClose));

	hbox->Add(createButton, 1);
	hbox->Add(closeButton, 1, wxLEFT, 5);

	vbox->Add(panel, 1);
	vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

	SetSizer(vbox);

	Centre();
	ShowModal();
	Destroy();
}

void NewMazeDialog::OnCreate(wxCommandEvent& event) {
	long l_rows, l_cols;
	if (rowEntry->GetValue().ToLong(&l_rows, 10)) {
		rows = (int) l_rows;
	}
	if (columnEntry->GetValue().ToLong(&l_cols, 10)) {
		cols = (int) l_cols;
	}
	Close(false);
	maze = new Maze2d(rows, cols, algorithm);
}

void NewMazeDialog::OnClose(wxCommandEvent& event) {
	canceled = true;
	Close(false);
}

void NewMazeDialog::OnSetRB(wxCommandEvent& event) {
	algorithm = "rb";
}
void NewMazeDialog::OnSetP(wxCommandEvent& event) {
	algorithm = "p";
}
void NewMazeDialog::OnSetE(wxCommandEvent& event) {
	algorithm = "e";
}
void NewMazeDialog::OnSetBT(wxCommandEvent& event) {
	algorithm = "bt";
}
void NewMazeDialog::OnSetAB(wxCommandEvent& event) {
	algorithm = "ab";
}
void NewMazeDialog::OnSetW(wxCommandEvent& event) {
	algorithm = "w";
}
void NewMazeDialog::OnSetS(wxCommandEvent& event) {
	algorithm = "s";
}
