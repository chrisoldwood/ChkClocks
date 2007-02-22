import java.awt.*;
import gort.ui.*;

/********************************************************************************
** This dialog is used to allow the user to load and save the puzzle.
*/

public class LoadSaveDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor.
	*/

	public LoadSaveDlg(Component oParent, String strTitle, String strPuzzle)
	{
		super(oParent, strTitle, OK_CANCEL);

		// Save parameters.
		m_strPuzzle = strPuzzle;

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.NORTH,  m_lblText);
		m_pnlControls.add(BorderLayout.CENTER, m_ebPuzzle);
		setResizable(true);

		// Display the puzzle.
		m_ebPuzzle.setText(m_strPuzzle);
		m_ebPuzzle.setFont(new Font("Monospaced", Font.PLAIN, 12));
	}

	/********************************************************************************
	** OK button pressed.
	*/

	public void onOK()
	{
		m_strPuzzle = m_ebPuzzle.getText();

		super.onOK();
	}

	/********************************************************************************
	** Constants.
	*/

	public static final String MSG =

	"To load and save puzzles you can copy and paste text between\n" +
	"Notepad and the window below. This is done to avoid having to\n" +
	"to get the applet signed.";

	/********************************************************************************
	** Members.
	*/

	// Data members.
	public String		m_strPuzzle = "";

	// GUI members.
	private	LabelEx		m_lblText  = new LabelEx(MSG);
	private TextArea	m_ebPuzzle = new TextArea("", 25, 50, TextArea.SCROLLBARS_BOTH);
}
