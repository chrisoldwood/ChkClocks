import java.awt.*;
import java.awt.event.*;
import java.applet.*;

/********************************************************************************
** This is the base class for all modal dialogs.
*/

public class ModalDialog extends Dialog
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public ModalDialog(Component oParent, String strTitle, int nButtons)
	{
		super(getParentFrame(oParent), strTitle, true);

		setResizable(false);
		setLayout(new BorderLayout(5, 5));

		// Create this panel.
		add(BorderLayout.CENTER, m_pnlControls);
		add(BorderLayout.SOUTH,  m_pnlButtons);
		setButtons(nButtons);

		// Add handler for the "X" close button.
		addWindowListener(new CloseListener());

		// Check if applet is direct parent, and enabled.
		// (Required because modal dialog does not disable it).
		while ( (!(oParent instanceof Frame)) && (!(oParent instanceof Dialog)) 
			 && (!(oParent instanceof Applet)) )
		{
			oParent = oParent.getParent();
		}

		if ( (oParent instanceof Applet) && (oParent.isEnabled()) )
			m_oParent = oParent;
	}

	/********************************************************************************
	** Button event handler.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		if (eEvent.getSource() == m_btnOK)		onOK();
		if (eEvent.getSource() == m_btnCancel)	onCancel();
		if (eEvent.getSource() == m_btnHelp)	onHelp();
	}

	/********************************************************************************
	** Event handler template methods.
	*/

	public void onOK()
	{
		m_nResult = OK;
		dispose();
	}

	public void onCancel()
	{
		m_nResult = CANCEL;
		dispose();
	}

	public void onHelp()
	{
	}

	/********************************************************************************
	** The dialog is about to be shown. Calculate the layout and centre.
	*/

	public void show()
	{
		// Layout controls.
		pack();
		centre();

		// Disable parent, if required.
		if (m_oParent != null)
			m_oParent.setEnabled(false);

		super.show();
	}

	/********************************************************************************
	** The dialog is about to be closed.
	*/

	public void dispose()
	{
		// Reenable parent, if required.
		if (m_oParent != null)
			m_oParent.setEnabled(true);

		super.dispose();
	}

	/********************************************************************************
	** Gets the dialog button used to close it.
	*/

	public int result()
	{
		return m_nResult;
	}

	/********************************************************************************
	** Returns the windows' border.
	*/

	public Insets getInsets()
	{
		return new Insets(30, 15, 20, 15);
	}

	/********************************************************************************
	** Sets up the array of buttons.
	*/

	private void setButtons(int nButtons)
	{
		// Derived class setting buttons?
		if (nButtons == NONE)
			return;

		// Close is usually on its own
		// and equivalent to Cancel.
		if ((nButtons & CLOSE) > 0)
		{
			m_btnCancel = new Button(CLOSE_LABEL);
			m_pnlButtons.add(m_btnCancel);
			m_btnCancel.addActionListener(this);
		}

		// OK is always leftmost.
		if ((nButtons & OK) > 0)
		{
			m_btnOK = new Button(OK_LABEL);
			m_pnlButtons.add(m_btnOK);
			m_btnOK.addActionListener(this);
		}

		// Cancel is always to the right.
		if ((nButtons & CANCEL) > 0)
		{
			m_btnCancel = new Button(CANCEL_LABEL);
			m_pnlButtons.add(m_btnCancel);
			m_btnCancel.addActionListener(this);
		}

		// Help is always rightmost.
		if ((nButtons & HELP) > 0)
		{
			m_btnHelp = new Button(HELP_LABEL);
			m_pnlButtons.add(m_btnHelp);
			m_btnHelp.addActionListener(this);
		}
	}

	/********************************************************************************
	** Centre the dialog on the parent.
	*/

	private void centre()
	{
		Dimension dmDialog = getSize();
		Dimension dmFrame  = getParent().getSize();
		Point	  ptFrame  = getParent().getLocationOnScreen();

		int x = ((dmFrame.width  - dmDialog.width)  / 2) + ptFrame.x;
		int y = ((dmFrame.height - dmDialog.height) / 2) + ptFrame.y;

		setLocation(x, y);
	}

	/********************************************************************************
	** Adapter used to listen for the user hitting the "X" close window button.
	*/

	private class CloseListener extends WindowAdapter
	{
		public void windowClosing(WindowEvent e)
		{
			onCancel();
		}
	}

	/********************************************************************************
	** Finds the parent frame for the component.
	*/

	private static Frame getParentFrame(Component c)
	{
		while(!(c instanceof Frame))
			c = c.getParent();

		return (Frame) c;
	}

	/********************************************************************************
	** Constants.
	*/

	// Individual button type masks.
	public static final int NONE   = 0x00;
	public static final int CLOSE  = 0x01;
	public static final int OK     = 0x02;
	public static final int CANCEL = 0x04;
	public static final int HELP   = 0x08;

	// Common button combinations.
	public static final int OK_CANCEL      = (OK | CANCEL);
	public static final int OK_CANCEL_HELP = (OK | CANCEL | HELP);

	// Button labels.
	protected static final String CLOSE_LABEL  = " Close  ";
	protected static final String OK_LABEL     = "   OK   ";
	protected static final String CANCEL_LABEL = " Cancel ";
	protected static final String HELP_LABEL   = "  Help  ";

	/********************************************************************************
	** Members.
	*/

	// The parent, if disabled manually.
	private Component  		m_oParent;

	// The child panels.
	protected Panel			m_pnlControls = new Panel(new BorderLayout(5, 5));
	protected ButtonPanel 	m_pnlButtons  = new ButtonPanel();

	// The buttons.
	protected Button		m_btnOK;
	protected Button		m_btnCancel;
	protected Button		m_btnHelp;

	// The result code.
	protected int			m_nResult = NONE;
}
