package gort.ui;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;

/********************************************************************************
** This is a dialog based class used to simulate Windows message boxes.
*/

public class MsgBox extends ModalDialog
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public MsgBox(Component oParent, String strTitle, String strMsg, int nButtons, int nIcon)
	{
		super(oParent, strTitle, NONE);

		// Create the icons image, if not already.
		if (s_imgIcons == null)
		{
			s_imgIcons = createImage(new MemoryImageSource(MsgBoxIcons.WIDTH, MsgBoxIcons.HEIGHT,
															MsgBoxIcons.PIXELS, 0, MsgBoxIcons.WIDTH));
		}

		// Construct the dialog body.
		m_pnlBody.add(BorderLayout.WEST,   m_icnIcon);
		m_pnlBody.add(BorderLayout.CENTER, m_lblText);
		m_pnlControls.add(BorderLayout.CENTER, m_pnlBody);
		setButtons(nButtons);

		// Setup the icon and message.
		m_lblText.setText(strMsg);
		m_icnIcon.setImage(s_imgIcons, new Rectangle((ICON_HEIGHT * nIcon), 0, ICON_WIDTH, ICON_HEIGHT));
	}

	/********************************************************************************
	** Sets up the array of buttons.
	*/

	private void setButtons(int nButtons)
	{
		// OK is always leftmost.
		if ((nButtons & OK) > 0)
		{
			m_btnOK = new Button(OK_LABEL);
			m_pnlButtons.add(m_btnOK);
			m_btnOK.addActionListener(this);
		}

		// Yes is always leftmost.
		if ((nButtons & YES) > 0)
		{
			m_btnYes = new Button(YES_LABEL);
			m_pnlButtons.add(m_btnYes);
			m_btnYes.addActionListener(this);
		}

		// No is always to the right.
		if ((nButtons & NO) > 0)
		{
			m_btnNo = new Button(NO_LABEL);
			m_pnlButtons.add(m_btnNo);
			m_btnNo.addActionListener(this);
		}

		// Cancel is always further right.
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
	** Button event handler.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		if (eEvent.getSource() == m_btnYes)
			onYes();
		else if (eEvent.getSource() == m_btnNo)
			onNo();
		else
			super.actionPerformed(eEvent);
	}

	/********************************************************************************
	** Event handler template methods.
	*/

	public void onYes()
	{
		m_nResult = YES;
		dispose();
	}

	public void onNo()
	{
		m_nResult = NO;
		dispose();
	}

	/********************************************************************************
	** Helpers methods to encapsulate the construction, display and result code.
	*/

	public static int display(Component oParent, String strTitle, String strMsg, int nButtons, int nIcon)
	{
		MsgBox oDlg = new MsgBox(oParent, strTitle, strMsg, nButtons, nIcon);

		return oDlg.prompt();
	}

	/********************************************************************************
	** Helper methods for common types of message boxes.
	*/

	public static int query(Component oParent, String strTitle, String strMsg, int nButtons)
	{
		return display(oParent, strTitle, strMsg, nButtons, QUESTION);
	}

	public static void notify(Component oParent, String strTitle, String strMsg)
	{
		display(oParent, strTitle, strMsg, OK, INFORMATION);
	}

	public static void alert(Component oParent, String strTitle, String strMsg)
	{
		display(oParent, strTitle, strMsg, OK, EXCLAMATION);
	}

	public static void fatal(Component oParent, String strTitle, String strMsg)
	{
		display(oParent, strTitle, strMsg, OK, STOP);
	}

	/********************************************************************************
	** Constants.
	*/

	// Icons.
	public static final int	STOP        = 0;
	public static final int	EXCLAMATION = 1;
	public static final int	QUESTION    = 2;
	public static final int	INFORMATION	= 3;

	// Individual button type masks.
	public static final int OK     = ModalDialog.OK;
	public static final int CANCEL = ModalDialog.CANCEL;
	public static final int HELP   = ModalDialog.HELP;
	public static final int YES    = 0x10;
	public static final int NO     = 0x20;

	// Common button combinations.
	public static final int OK_CANCEL      = ModalDialog.OK_CANCEL;
	public static final int OK_CANCEL_HELP = ModalDialog.OK_CANCEL_HELP;
	public static final int YES_NO         = (YES | NO);
	public static final int YES_NO_CANCEL  = (YES | NO | CANCEL);

	// Button labels.
	private static final String YES_LABEL = "  Yes   ";
	private static final String NO_LABEL  = "   No   ";

	// Icon dims.
	public static final int ICON_WIDTH  = 32;
	public static final int ICON_HEIGHT = 32;

	/********************************************************************************
	** Members.
	*/

	// The dialog controls.
	private BorderPanel	m_pnlBody = new BorderPanel(new BorderLayout(5, 5));
	private Icon		m_icnIcon = new Icon();
	private	LabelEx 	m_lblText = new LabelEx();

	// The buttons.
	private Button	m_btnYes;
	private Button	m_btnNo;

	// The icons image.
	private static Image s_imgIcons;
}
