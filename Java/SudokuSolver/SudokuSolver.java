import java.awt.*;
import java.applet.*;
import gort.ui.*;

/********************************************************************************
** The applet derived class used as the entry point for the applet.
*/

public class SudokuSolver extends Applet
{
	/********************************************************************************
	** Template method called by the VM to initialise the applet.
	*/

	public void init()
	{
		try
		{
			// Initialise the applets panel.
			setLayout(new BorderLayout());
			setBackground(SystemColor.control);

			// Layout the applet panel.
			add(BorderLayout.NORTH,  m_oTitleBar);
			add(BorderLayout.SOUTH,  m_oMainCmds);
			add(BorderLayout.CENTER, m_oPuzzleGrid);
		}
		catch (Exception e)
		{
			MsgBox.fatal(this, APPLET_NAME, e.toString());
			e.printStackTrace();
		}
	}

	/********************************************************************************
	** Template method called by the VM to start the applet running.
	*/

	public void start()
	{
	}

	/********************************************************************************
	** Template method called by the VM to stop the applet running.
	*/

	public void stop()
	{
	}

	/********************************************************************************
	** Template method called by the VM to terminate the applet.
	*/

	public void destroy()
	{
	}

	/********************************************************************************
	** Constants
	*/

	public static final String APPLET_NAME = "Sudoku Solver";

	/********************************************************************************
	** Members.
	*/

	// GUI panels.
	public TitleBar			m_oTitleBar   = new TitleBar();
	public PuzzleGrid		m_oPuzzleGrid = new PuzzleGrid(this);
	public MainCmdsPanel	m_oMainCmds   = new MainCmdsPanel(this);
}
