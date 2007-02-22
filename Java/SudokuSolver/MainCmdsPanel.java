import java.awt.*;
import java.awt.event.*;
import gort.ui.*;

/********************************************************************************
** This is the panel that hosts the main command buttons etc.
*/

public class MainCmdsPanel extends Panel
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public MainCmdsPanel(SudokuSolver oApplet)
	{
		// Initialise members.
		m_oApplet = oApplet;
		m_oPuzzle = m_oApplet.m_oPuzzleGrid;

		// Create the layout panels.
		m_pnlLeft.add(m_btnNew);
		m_pnlLeft.add(m_btnLoad);
		m_pnlLeft.add(m_btnSave);
		m_pnlLeft.add(m_btnStart);
		m_pnlLeft.add(m_btnReset);

		m_pnlMiddle.add(m_btnCheck);
		m_pnlMiddle.add(m_btnHint);
		m_pnlMiddle.add(m_btnSolve);

		m_pnlRight.add(m_btnHelp);

		// Initialise this panel.
		setLayout(new BorderLayout());
		setBackground(SystemColor.control);

		add(BorderLayout.WEST,   m_pnlLeft);
		add(BorderLayout.CENTER, m_pnlMiddle);
		add(BorderLayout.EAST,   m_pnlRight);
		add(BorderLayout.SOUTH,  m_lblHintBar);

		// Add the event handlers.
		m_btnNew.addActionListener(this);
		m_btnLoad.addActionListener(this);
		m_btnSave.addActionListener(this);
		m_btnStart.addActionListener(this);
		m_btnReset.addActionListener(this);
		m_btnCheck.addActionListener(this);
		m_btnHint.addActionListener(this);
		m_btnSolve.addActionListener(this);
		m_btnHelp.addActionListener(this);

		m_btnNew.addMouseListener(new ButtonMouseListener());
		m_btnLoad.addMouseListener(new ButtonMouseListener());
		m_btnSave.addMouseListener(new ButtonMouseListener());
		m_btnStart.addMouseListener(new ButtonMouseListener());
		m_btnReset.addMouseListener(new ButtonMouseListener());
		m_btnCheck.addMouseListener(new ButtonMouseListener());
		m_btnHint.addMouseListener(new ButtonMouseListener());
		m_btnSolve.addMouseListener(new ButtonMouseListener());
		m_btnHelp.addMouseListener(new ButtonMouseListener());

		for (int y = 0; y < PuzzleGrid.PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PuzzleGrid.PUZZLE_SIZE; ++x)
			{
				m_oPuzzle.getSquare(x, y).addMouseListener(new SquareMouseListener());
			}
		}

		// Initiaise buitton states.
		UpdateButtons();
	}

	/********************************************************************************
	** Set the hint in the status bar.
	*/

	public void setHint(String str)
	{
		m_lblHintBar.setText(str);
	}

	/********************************************************************************
	** The raw button event handler.
	*/

	public void actionPerformed(ActionEvent oEvent)
	{
		try
		{
			onButtonClicked((Button) oEvent.getSource());
		}
		catch (Exception e)
		{
			MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
			e.printStackTrace();
		}
	}

	/********************************************************************************
	** The button event handler.
	*/

	private void onButtonClicked(Button oButton)
	{
		// Handle button press.
		if (oButton == m_btnNew)
		{
			m_oPuzzle.definePuzzle();
		}
		else if (oButton == m_btnLoad)
		{
			m_oPuzzle.loadPuzzle();
		}
		else if (oButton == m_btnSave)
		{
			m_oPuzzle.savePuzzle();
		}
		else if (oButton == m_btnStart)
		{
			m_oPuzzle.startPuzzle();
		}
		else if (oButton == m_btnReset)
		{
			m_oPuzzle.resetPuzzle();
		}
		else if (oButton == m_btnCheck)
		{
			m_oPuzzle.checkPuzzle();
		}
		else if (oButton == m_btnHelp)
		{
			MsgBox.notify(m_oApplet, SudokuSolver.APPLET_NAME, HELP);
		}

		// Update UI.
		UpdateButtons();
	}

	/********************************************************************************
	** Update the button states.
	*/

	private void UpdateButtons()
	{
		// Disable most of them by default
		m_btnNew.setEnabled(false);
		m_btnLoad.setEnabled(false);
		m_btnSave.setEnabled(false);
		m_btnStart.setEnabled(false);
		m_btnReset.setEnabled(false);
		m_btnCheck.setEnabled(false);
		m_btnHint.setEnabled(false);
		m_btnSolve.setEnabled(false);
		m_btnHelp.setEnabled(true);

		int nUserState = m_oPuzzle.getUserState();

		// Enable those buttons available.
		if (nUserState == PuzzleGrid.INITIALISED)
		{
			m_btnNew.setEnabled(true);
			m_btnLoad.setEnabled(true);
		}
		else if (nUserState == PuzzleGrid.DEFINING_PUZZLE)
		{
			m_btnSave.setEnabled(true);
			m_btnStart.setEnabled(true);
			m_btnCheck.setEnabled(true);
		}
		else if (nUserState == PuzzleGrid.SOLVING_PUZZLE)
		{
			m_btnNew.setEnabled(true);
			m_btnLoad.setEnabled(true);
			m_btnSave.setEnabled(true);
			m_btnReset.setEnabled(true);
			m_btnCheck.setEnabled(true);
		}
	}

	/********************************************************************************
	** Event handlers for when the mouse hovers over a control.
	*/

	private void onHoverButton(Button oButton, boolean bEntered)
	{
		// Remove hint by default.
		String str = "";

		// Set hint if entering window.
		if (bEntered)
		{
			if      (oButton == m_btnNew)
				str = "Click to enter a new puzzle to solve";
			else if (oButton == m_btnLoad)
				str = "Click to load a puzzle";
			else if (oButton == m_btnSave)
				str = "Click to save the current puzzle";
			else if (oButton == m_btnStart)
				str = "Click to begin solving the puzzle";
			else if (oButton == m_btnReset)
				str = "Click to remove your answers so that you can start again";
			else if (oButton == m_btnCheck)
				str = "Click to check that the numbers you have enetered are valid";
			else if (oButton == m_btnHint)
				str = "Click to provide a hint about which square to answer next";
			else if (oButton == m_btnSolve)
				str = "Click to automatically solve the puzzle";
			else if (oButton == m_btnHelp)
				str = "Click to display the help window";
		}

		setHint(str);
	}

	private void onHoverSquare(PuzzleSquare oSquare, boolean bEntered)
	{
		// Remove hint by default.
		String str = "";

		// Set hint if entering window.
		if (bEntered)
		{
			if (m_oPuzzle.getUserState() == PuzzleGrid.DEFINING_PUZZLE)
				str = "Press 1..9 to enter the predefined answer into the square or Delete to remove it";
			else if (m_oPuzzle.getUserState() == PuzzleGrid.SOLVING_PUZZLE)
				str = "Press 1..9 to enter an answer, Ctrl+1..9 to enter possible answers or Delete to remove it";
		}

		setHint(str);
	}

	/********************************************************************************
	** Adapter used to listen for mouse enter/leave events for the buttons.
	*/

	private class ButtonMouseListener extends MouseAdapter
	{
		public void mouseEntered(MouseEvent oEvent)
		{
			try
			{
				onHoverButton((Button)oEvent.getSource(), true);
			}
			catch (Exception e)
			{
				MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
				e.printStackTrace();
			}
		}

		public void mouseExited(MouseEvent oEvent)
		{
			try
			{
				onHoverButton((Button)oEvent.getSource(), false);
			}
			catch (Exception e)
			{
				MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
				e.printStackTrace();
			}
		}
	}

	/********************************************************************************
	** Adapter used to listen for mouse enter/leave events for the puzzle squares.
	*/

	private class SquareMouseListener extends MouseAdapter
	{
		public void mouseEntered(MouseEvent oEvent)
		{
			try
			{
				onHoverSquare((PuzzleSquare)oEvent.getSource(), true);
			}
			catch (Exception e)
			{
				MsgBox.fatal(m_oApplet, SudokuSolver.APPLET_NAME, e.toString());
				e.printStackTrace();
			}
		}

		public void mouseExited(MouseEvent oEvent)
		{
			try
			{
				onHoverSquare((PuzzleSquare)oEvent.getSource(), false);
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

	public static final String HELP =

	"First enter the predefined puzzle answers into the grid by clicking\n" +
	"the relevant squares and using the number keys 1..9.\n" +
	"\n" +
	"Next click the 'Start' button to begin completing the puzzle. Once\n" +
	"again click the squares (or use the cursor keys) and press the\n" +
	"number keys 1..9 to enter your answers. You can remove an answer\n" +
	"by pressing Delete. If you have more than one possible answer\n" +
	"you can enter them all by pressing Ctrl and 1..9.\n" +
	"\n" +
	"If you want to check that you haven't made a mistake you can use\n" +
	"the 'Check' button to highlight errors. You can start again by pressing\n" +
	"'Reset', or enter a different puzzle by pressing 'New Puzzle'."+
	"";

	/********************************************************************************
	** Members.
	*/

	// Data members.
	private SudokuSolver	m_oApplet = null;
	private PuzzleGrid		m_oPuzzle = null;

	// GUI Controls
	private Button		m_btnNew     = new Button("New");
	private Button		m_btnLoad    = new Button("Load");
	private Button		m_btnSave    = new Button("Save");

	private Button		m_btnStart   = new Button("Start");
	private Button		m_btnReset   = new Button("Reset");

 	private Button		m_btnCheck   = new Button("Check");
	private Button		m_btnHint    = new Button("Hint");
	private Button		m_btnSolve   = new Button("Solve");

	private Button		m_btnHelp    = new Button("Help");

	private Label		m_lblHintBar = new Label("");
	
	// Layout panels
	private Panel		m_pnlLeft   = new Panel(new FlowLayout(FlowLayout.LEFT));
	private Panel		m_pnlMiddle = new Panel(new FlowLayout(FlowLayout.CENTER));
	private Panel		m_pnlRight  = new Panel(new FlowLayout(FlowLayout.RIGHT));
}
