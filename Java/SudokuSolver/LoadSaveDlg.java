import java.awt.*;
import gort.ui.*;

/********************************************************************************
** This dialog is used to allow the user to load and save the puzzle.
*/

public class LoadSaveDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor for use when loading a puzzle.
	*/

	public LoadSaveDlg(Component oParent)
	{
		this(oParent, "Load Puzzle", OK_CANCEL, null);
	}

	/********************************************************************************
	** Constructor for use when saving a puzzle.
	*/

	public LoadSaveDlg(Component oParent, String strPuzzle)
	{
		this(oParent, "Save Puzzle", CLOSE, strPuzzle);
	}

	/********************************************************************************
	** Constructor.
	*/

	public LoadSaveDlg(Component oParent, String strTitle, int nButtons, String strPuzzle)
	{
		super(oParent, strTitle, nButtons);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.NORTH,  m_lblText);
		m_pnlControls.add(BorderLayout.CENTER, m_ebPuzzle);
		setResizable(true);

		// Display the puzzle.
		m_ebPuzzle.setFont(new Font("Monospaced", Font.PLAIN, 12));

		if (strPuzzle != null)
			m_ebPuzzle.setText(strPuzzle);
	}

	/********************************************************************************
	** OK button pressed.
	*/

	public void onOK()
	{
		m_strPuzzle = m_ebPuzzle.getText();

		// Quickly validate the text.
		if (!PuzzleSerialiser.validate(m_strPuzzle))
		{
			final String MSG =  "The puzzle does not appear to be complete.\n\n" +
								"Do you want to continue and load it?";

			if (MsgBox.display(this, SudokuSolver.APPLET_NAME, MSG, MsgBox.YES_NO, MsgBox.EXCLAMATION) != MsgBox.YES)
				return;
		}

		super.onOK();
	}

	/********************************************************************************
	** Constants.
	*/

	public static final String MSG =

	"To load and save puzzles you can copy and paste text between Notepad\n" +
	"and the window below. This is done to avoid having to 'sign' the applet.\n";

	/********************************************************************************
	** Members.
	*/

	// Data members.
	public String		m_strPuzzle = null;

	// GUI members.
	private	LabelEx		m_lblText  = new LabelEx(MSG);
	private TextArea	m_ebPuzzle = new TextArea("", 12, 50, TextArea.SCROLLBARS_BOTH);
}
