import java.awt.*;
import java.awt.event.*;
import gort.ui.*;

/********************************************************************************
** This is the panel that hosts the main puzzle grid.
*/

public class PuzzleGrid extends Panel
{
	/********************************************************************************
	** Constructor.
	*/

	public PuzzleGrid(SudokuSolver oApplet)
	{
		// Initialise members.
		m_oApplet = oApplet;

		// Initialise this panel.
		setLayout(new GridLayout(PUZZLE_SIZE, PUZZLE_SIZE, 0, 0));
		setBackground(SystemColor.control);

		// Add the puzzle squares.
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				PuzzleSquare oSquare = new PuzzleSquare(this, new Point(x, y));

				// Add the square to the grid.
				m_aoSquares[x][y] = oSquare;
				add(oSquare);

				// Attach the event handlers.
				oSquare.addMouseListener(new SquareMouseListener());
				oSquare.addKeyListener(new SquareKeyListener());
				oSquare.addFocusListener(new SquareFocusListener());
			}
		}
	}

	/********************************************************************************
	** Get the square at the position specified.
	*/

	public PuzzleSquare getSquare(int x, int y)
	{
		return m_aoSquares[x][y];
	}

	/********************************************************************************
	** Get the state that the user is in. This determines what actions can be done
	** by the user.
	*/

	public int getUserState()
	{
		return m_nUserState;
	}

	/********************************************************************************
	** Start defining a new puzzle. In this mode the user enters the numbers that
	** are already provided by the puzzle.
	*/

	public void definePuzzle()
	{
		// Clear all squares...
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				m_aoSquares[x][y].clear(false);
			}
		}

		m_nUserState = DEFINING_PUZZLE;
	}

	/********************************************************************************
	** Load a puzzle from a text stream.
	*/

	public void loadPuzzle()
	{
		LoadSaveDlg Dlg = new LoadSaveDlg(m_oApplet, "Load Puzzle", "");

		if (Dlg.prompt() == Dlg.OK)
		{
			SquareState[][] aoStates = PuzzleSerialiser.read(Dlg.m_strPuzzle);

			// Update all squares...
			for (int y = 0; y < PUZZLE_SIZE; ++y)
			{
				for (int x = 0; x < PUZZLE_SIZE; ++x)
				{
					if (aoStates[x][y] != null)
						m_aoSquares[x][y].setState(aoStates[x][y]);
					else
						m_aoSquares[x][y].clear(false);
				}
			}

			clearStyles();

			m_nUserState = SOLVING_PUZZLE;
		}
	}

	/********************************************************************************
	** Save the puzzle to a text stream.
	*/

	public void savePuzzle()
	{
		String str = PuzzleSerialiser.write(m_aoSquares);

		LoadSaveDlg Dlg = new LoadSaveDlg(m_oApplet, "Save Puzzle", str);

		Dlg.prompt();
	}

	/********************************************************************************
	** Start to manually solve the puzzle. This is the mode entered when the user is
	** trying to solve it themselves.
	*/

	public void startPuzzle()
	{
		if (!checkPuzzle())
			return;

		clearStyles();

		m_nUserState = SOLVING_PUZZLE;
	}

	/********************************************************************************
	** Reset the puzzle back to the point where only the predfined numbers exist.
	*/

	public void resetPuzzle()
	{
		// Reset all squares...
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				PuzzleSquare oSquare = m_aoSquares[x][y];

				oSquare.clear(true);
				oSquare.setStyle(PuzzleSquare.NORMAL_STYLE);
			}
		}
	}

	/********************************************************************************
	** Check that both the predefined and user supplied answers are valid.
	*/

	public boolean checkPuzzle()
	{
		boolean[][]   abErrors = new boolean[PUZZLE_SIZE][PUZZLE_SIZE];
		PuzzleChecker oChecker = new PuzzleChecker(m_aoSquares, abErrors);

		// Run the checker.
		int nErrors = oChecker.check();

		// Highlight/clear errors.
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				if (abErrors[x][y])
					m_aoSquares[x][y].setStyle(PuzzleSquare.ERROR_STYLE);
				else
					m_aoSquares[x][y].setStyle(PuzzleSquare.NORMAL_STYLE);
			}
		}

		return (nErrors == 0);
	}

	/********************************************************************************
	** Clear all styles.
	*/

	public void clearStyles()
	{
		// Reset all squares...
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				m_aoSquares[x][y].setStyle(PuzzleSquare.NORMAL_STYLE);
			}
		}
	}

	/********************************************************************************
	** Restore the keyboard focus to the selected puzzle square as it may have been
	** taken by a button click.
	*/

	public void restoreFocus()
	{
		if ( (m_ptSelection.x != -1) && (m_ptSelection.y != -1) )
			m_aoSquares[m_ptSelection.x][m_ptSelection.y].requestFocus();
	}

	/********************************************************************************
	** Get the font used for the small text. This is the default GUI font.
	*/

	public Font getSmallFont()
	{
		if (m_oSmallFont == null)
			m_oSmallFont = getGraphics().getFont();

		return m_oSmallFont;
	}

	/********************************************************************************
	** Get the font used for the big text. This font is twice as big as the default
	** GUI font.
	*/

	public Font getBigFont()
	{
		if (m_oBigFont == null)
		{
			Font oDefFont = getGraphics().getFont();

			m_oBigFont = new Font(oDefFont.getName(), oDefFont.getStyle(), oDefFont.getSize() * 3);
		}

		return m_oBigFont;
	}

	/********************************************************************************
	** The mouse was clicked on one of the squares. Change the selected square.
	*/

	private void onMouseClicked(PuzzleSquare oSquare)
	{
		// Ignore if input not allowed yet.
		if (m_nUserState == INITIALISED)
			return;

		if ( (m_ptSelection.x != -1) && (m_ptSelection.y != -1) )
			m_aoSquares[m_ptSelection.x][m_ptSelection.y].setSelection(false);

		m_ptSelection = new Point(oSquare.getPosition());

		m_aoSquares[m_ptSelection.x][m_ptSelection.y].setSelection(true);
	}

	/********************************************************************************
	** The cursor keys were used within a squeare. Change the selected square.
	*/

	private void onCursorKeyReleased(PuzzleSquare oSquare, int nKey)
	{
		// Ignore if input not allowed yet.
		if (m_nUserState == INITIALISED)
			return;

		// Calculate new selection.
		Point ptNewSelection = new Point(m_ptSelection);

		switch (nKey) 
		{
			case KeyEvent.VK_LEFT:		ptNewSelection.x += -1;				break;
			case KeyEvent.VK_RIGHT:		ptNewSelection.x +=  1;				break;
			case KeyEvent.VK_UP:		ptNewSelection.y += -1;				break;
			case KeyEvent.VK_DOWN:		ptNewSelection.y +=  1;				break;

			case KeyEvent.VK_HOME:		ptNewSelection.x = 0;				break;
			case KeyEvent.VK_END:		ptNewSelection.x = PUZZLE_SIZE-1;	break;
			case KeyEvent.VK_PAGE_UP:	ptNewSelection.y = 0;				break;
			case KeyEvent.VK_PAGE_DOWN:	ptNewSelection.y = PUZZLE_SIZE-1;	break;
		}

		// If valid square, change selection.
		if ( (ptNewSelection.x >= 0) && (ptNewSelection.x < PUZZLE_SIZE)
		  && (ptNewSelection.y >= 0) && (ptNewSelection.y < PUZZLE_SIZE) )
		{
			m_aoSquares[m_ptSelection.x][m_ptSelection.y].setSelection(false);

			m_ptSelection = ptNewSelection;

			m_aoSquares[m_ptSelection.x][m_ptSelection.y].setSelection(true);
		}
	}

	/********************************************************************************
	** The number keys were used within a squeare. Change the square's answer or
	** list of possible choices. This is only valid when defining or solving the
	** puzzle.
	*/

	private void onNumberKeyReleased(PuzzleSquare oSquare, int nKey, boolean bShift, boolean bCtrl)
	{
		// Ignore if input not allowed yet.
		if (m_nUserState == INITIALISED)
			return;

		int     nAnswer   = nKey - KeyEvent.VK_0;
		boolean bDefining = (m_nUserState == DEFINING_PUZZLE);
		boolean bSolving  = (m_nUserState == SOLVING_PUZZLE);

		// Entering an answer?
		if ( (!bShift && !bCtrl) && (bDefining || bSolving) )
		{
			if ( (bDefining) || (bSolving && !oSquare.getState().isPredefinedValue()) )
				oSquare.setAnswer(nAnswer, bDefining);
		}
		// Entering a choice?
		if ( (bCtrl && !bShift) && (bSolving) )
		{
			if (bSolving && !oSquare.getState().isPredefinedValue())
			{
				SquareState oState    = oSquare.getState();
				boolean[]   abChoices = null;

				if (oState.getState() == SquareState.USER_CHOICES)
					abChoices = oState.getChoices();
				else
					abChoices = new boolean[PUZZLE_SIZE];

				// Toggle choice.
				abChoices[nAnswer-1] = !abChoices[nAnswer-1];

				oSquare.setChoices(abChoices);
			}
		}
	}

	/********************************************************************************
	** The Delete key was used within a squeare. Delete any user provided answer.
	*/

	private void onDeleteKeyReleased(PuzzleSquare oSquare)
	{
		// Ignore if input not allowed yet.
		if (m_nUserState == INITIALISED)
			return;

		boolean bDefining   = (m_nUserState == DEFINING_PUZZLE);
		boolean bSolving    = (m_nUserState == SOLVING_PUZZLE);
		boolean bPredefined = oSquare.getState().isPredefinedValue();

		if (bDefining)
			oSquare.clear(false);
		else if (bSolving && !bPredefined)
			oSquare.clear(true);
	}

	/********************************************************************************
	** Adapter used to listen for just mouse click events.
	*/

	private class SquareMouseListener extends MouseAdapter
	{
		public void mouseClicked(MouseEvent oEvent)
		{
			try
			{
				PuzzleSquare oSquare = (PuzzleSquare)oEvent.getSource();

				onMouseClicked(oSquare);
			}
			catch (Exception e)
			{
				MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
				e.printStackTrace();
			}
		}
	}

	/********************************************************************************
	** Adapter used to listen for key events.
	*/

	private class SquareKeyListener extends KeyAdapter
	{
		public void keyReleased(KeyEvent oEvent)
		{
			try
			{
				int nKey = oEvent.getKeyCode();

				// Selection navigation key?
				if ( (nKey == KeyEvent.VK_LEFT)    || (nKey == KeyEvent.VK_RIGHT)
				  || (nKey == KeyEvent.VK_UP)      || (nKey == KeyEvent.VK_DOWN)
				  || (nKey == KeyEvent.VK_PAGE_UP) || (nKey == KeyEvent.VK_PAGE_DOWN)
				  || (nKey == KeyEvent.VK_HOME)    || (nKey == KeyEvent.VK_END) )
				{
					PuzzleSquare oSquare = (PuzzleSquare)oEvent.getSource();

					onCursorKeyReleased(oSquare, nKey);
				}
				// Number key?
				else if ( (nKey >= KeyEvent.VK_1) && (nKey <= KeyEvent.VK_9) )
				{
					PuzzleSquare oSquare = (PuzzleSquare)oEvent.getSource();

					onNumberKeyReleased(oSquare, nKey, oEvent.isShiftDown(), oEvent.isControlDown());
				}
				// Deleet key?
				else if ( (nKey == KeyEvent.VK_DELETE) || (nKey == KeyEvent.VK_BACK_SPACE) )
				{
					PuzzleSquare oSquare = (PuzzleSquare)oEvent.getSource();

					onDeleteKeyReleased(oSquare);
				}
			}
			catch (Exception e)
			{
				MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
				e.printStackTrace();
			}
		}
	}

	/********************************************************************************
	** Adapter used to listen for key events. When navigating by the cursor keys or
	** mouse these should not have any effect as the square should be in the correct
	** state. These come in use when clicking the command buttons, or when the host
	** is being activated/deactivated as it tries to restore the focus automatically.
	*/

	private class SquareFocusListener extends FocusAdapter
	{
		public void focusLost(FocusEvent oEvent)
		{
			// Ignore if input not allowed yet.
			if (m_nUserState == INITIALISED)
				return;

			try
			{
				PuzzleSquare oSquare = (PuzzleSquare)oEvent.getSource();

				// Is the selected square?
				if (oSquare.isSelected())
				{
					Point ptPosition = oSquare.getPosition();

					// Remove any selection.
					m_aoSquares[ptPosition.x][ptPosition.y].setSelection(false);

					m_ptSelection.x = -1;
					m_ptSelection.y = -1;
				}
			}
			catch (Exception e)
			{
				MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
				e.printStackTrace();
			}
		}

		public void focusGained(FocusEvent oEvent)
		{
			// Ignore if input not allowed yet.
			if (m_nUserState == INITIALISED)
				return;

			try
			{
				PuzzleSquare oSquare = (PuzzleSquare)oEvent.getSource();

				// Square regained focus?
				if (!oSquare.isSelected())
				{
					Point ptPosition = oSquare.getPosition();

					// Remove any selection.
					m_aoSquares[ptPosition.x][ptPosition.y].setSelection(true);

					m_ptSelection.x = ptPosition.x;
					m_ptSelection.y = ptPosition.y;
				}
			}
			catch (Exception e)
			{
				MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
				e.printStackTrace();
			}
		}
	}

	/********************************************************************************
	** Contants.
	*/

	public static final int PUZZLE_SIZE		= 9;

	public static final int INITIALISED		= 1;
	public static final int DEFINING_PUZZLE	= 2;
	public static final int SOLVING_PUZZLE	= 3;

	/********************************************************************************
	** Members.
	*/

		// Data members.
	private SudokuSolver		m_oApplet     = null;
	private PuzzleSquare[][]	m_aoSquares   = new PuzzleSquare[PUZZLE_SIZE][PUZZLE_SIZE];
	private Point				m_ptSelection = new Point(-1, -1);
	private int					m_nUserState  = INITIALISED; 

	// GUI members.
	private Font				m_oSmallFont  = null;
	private Font				m_oBigFont    = null;
}
