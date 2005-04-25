import java.awt.*;
import java.awt.event.*;
import gort.ui.*;
import gort.text.*;

/********************************************************************************
** This panel is used to host the program control options.
*/

public class ProgramPanel extends GroupPanel
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public ProgramPanel(Turtle oTurtle, Display oDisplay, Program oProgram)
	{
		super("Program", GroupPanel.CENTER, new BorderLayout(0, 5));

		// Save parameters.
		m_oTurtle  = oTurtle;
		m_oDisplay = oDisplay;
		m_oProgram = oProgram;

		// Create this panel.
		setBackground(SystemColor.control);
		add(BorderLayout.CENTER, m_pnlFields);

		// Create the child panels.
		m_pnlFields.add(m_btnClear,  m_btnEdit);
		m_pnlFields.add(m_btnExec,   m_cbSpeed);
		m_pnlFields.add(m_lblSpace1, m_lblSpace2);

		// Add the event handlers.
		m_btnClear.addActionListener(this);
		m_btnEdit.addActionListener(this);
		m_btnExec.addActionListener(this);

		// Add the speeds.
		m_cbSpeed.add("Fast");
		m_cbSpeed.add("Medium");
		m_cbSpeed.add("Slow");

		// Set defaults.
		m_cbSpeed.select(0);
	}

	/********************************************************************************
	** Button pressed.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		if (eEvent.getSource() == m_btnClear)
			onClear();
		if (eEvent.getSource() == m_btnEdit)
			onEdit();
		else if (eEvent.getSource() == m_btnExec)
			onExec();
	}

	/********************************************************************************
	** Clear action.
	*/

	public void onClear()
	{
		m_oTurtle.reset(true);
		m_oProgram.clear();
	}

	/********************************************************************************
	** Edit source action.
	*/

	public void onEdit()
	{
		// Generate the source code text.
		SourceLines oLines = m_oProgram.getSource();

		// Format.		
		StringBuffer sbSource = new StringBuffer(256);

		for(int i = 0; i < oLines.count(); i++)
			sbSource.append(oLines.getIndentedLine(i) + "\n");

		String  strSource = sbSource.toString();
		boolean bImported = false;

		// Until success or aborted.
		while (!bImported)
		{
			bImported = true;

			// Display.
			EditProgDlg Dlg = new EditProgDlg(this, strSource);

			if (Dlg.prompt() == Dlg.OK)
			{
				// Save source in case of error.
				strSource = Dlg.m_strSource;

				CmdFactory oCmdFactory = new CmdFactory();

				// Delete the existing program.
				m_oTurtle.reset(true);
				m_oProgram.clear();

				// Break the source code into lines.
				StrTok   st = new StrTok(true, true);
				String[] astrSource = st.splitLines(strSource);

				// Parse all lines.
				for (int i = 0; i < astrSource.length; i++)
				{
					try
					{
						m_oProgram.add(oCmdFactory.createCmd(astrSource[i]));
					}
					catch(InvalidCmdException e)
					{
						String strMsg = "Invalid command on line: " + (i+1) + "\n\n"
										+ e.getMessage() + "\n\n"
										+ "Continue with import?";

						if (MsgBox.query(this, "JLogo Import", strMsg, MsgBox.YES_NO) == MsgBox.NO)
						{
							bImported = false;
							break;
						}
					}
				}

				// Notify listeners of edited program.
				m_oProgram.notifyListeners(Program.EDITED);
			}
		}
	}

	/********************************************************************************
	** Execute action.
	*/

	public void onExec()
	{
		int nSpeed = m_cbSpeed.getSelectedIndex() * Program.SPEED_FACTOR;

		m_oProgram.execute(this, m_oTurtle, nSpeed);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private DualColPanel m_pnlFields = new DualColPanel();

	// Child controls.
	private Button		m_btnClear = new Button(" Clear ");
	private Button 		m_btnEdit  = new Button(" Edit... ");

	private Button		m_btnExec  = new Button(" Run ");
	private Choice		m_cbSpeed  = new Choice();

	private Label		m_lblSpace1 = new Label();
	private Label		m_lblSpace2 = new Label();

	// Data members.
	private Turtle		m_oTurtle;
	private Display		m_oDisplay;
	private Program		m_oProgram;
}
