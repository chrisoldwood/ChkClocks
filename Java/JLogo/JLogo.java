import java.applet.*;
import java.awt.*;
import java.awt.event.*;

/********************************************************************************
** The applet derived class used as the entry point for the applet.
*/

public class JLogo extends WebApp
	implements ActionListener
{
	/********************************************************************************
	** Template method called by the VM to initialise the applet.
	*/

	public void init()
	{
		// Create the GUI.
		setLayout(new BorderLayout());
		setBackground(Color.pink);

		add(BorderLayout.NORTH,  m_oTitleBar);
		add(BorderLayout.WEST,   m_oCmdsPanel);
		add(BorderLayout.SOUTH,  m_oCtrlPanel);
		add(BorderLayout.CENTER, m_oDisplay);

		// Add event handlers.
		m_oProgram.addActionListener(this);
	}

	/********************************************************************************
	** Template method called by the VM to start the applet running.
	*/

	public void start()
	{
		m_oTurtle.reset();
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
	** Action event handler.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		if (eEvent.getSource() == m_oProgram)
		{
			String strEvent = eEvent.getActionCommand();

			if (strEvent.equals(Program.START))
				onProgramStart();

			if (strEvent.equals(Program.STOP))
				onProgramStop();
		}
	}

	/********************************************************************************
	** Program event handlers.
	*/

	public void onProgramStart()
	{
		setEnabled(false);
	}

	public void onProgramStop()
	{
		setEnabled(true);
	}

	/********************************************************************************
	** Constants
	*/

	/********************************************************************************
	** Members.
	*/

	// Data members.
	private Program		m_oProgram   = new Program();

	// GUI members.
	private TitleBar	m_oTitleBar  = new TitleBar();
	private Display		m_oDisplay   = new Display();
	private Turtle		m_oTurtle    = new Turtle(m_oDisplay);
	private CmdsPanel	m_oCmdsPanel = new CmdsPanel(m_oTurtle, m_oProgram);
	private CtrlPanel	m_oCtrlPanel = new CtrlPanel(m_oTurtle, m_oDisplay, m_oProgram);
}
