import java.awt.*;
import java.awt.event.*;

/********************************************************************************
** This dialog is used to edit the source code.
*/

public class EditSourceDlg extends ModalDialog
	implements ItemListener
{
	/********************************************************************************
	** Constructor.
	*/

	public EditSourceDlg(Component oParent, Program oProgram)
	{
		super(oParent, "Edit Source Code", CLOSE);

		// Save parameters.
		m_oProgram = oProgram;

		// Create the child panels.
		m_pnlEdit.add(m_ebCmd);
		m_pnlEdit.add(m_ebParam);

		m_pnlBtns.add(m_btnRemove);
		m_pnlBtns.add(m_btnEdit);

		m_pnlCmd.add(BorderLayout.CENTER, m_pnlEdit);
		m_pnlCmd.add(BorderLayout.EAST,   m_pnlBtns);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.CENTER, m_lbSource);
		m_pnlControls.add(BorderLayout.SOUTH,  m_pnlCmd);

		setResizable(true);

		// Fill the listbox with source code.
		refreshListing();

		// Full command is not editable.
		m_ebCmd.setEditable(false);

		// Add the event handlers.
		m_lbSource.addItemListener(this);
		m_btnRemove.addActionListener(this);
		m_btnEdit.addActionListener(this);
	}

	/********************************************************************************
	** Queries if the source was modified.
	*/

	public boolean isSrcModified()
	{
		return m_bModified;
	}

	/********************************************************************************
	** Button pressed.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		try
		{
			if (eEvent.getSource() == m_btnRemove)
				onRemove();
			else if (eEvent.getSource() == m_btnEdit)
				onEdit();
		}
		catch(InvalidParamException ipe)
		{
			MsgBox.alert(this, "JLogo", ipe.getMessage());
			ipe.setFocus();
		}

		super.actionPerformed(eEvent);
	}

	/********************************************************************************
	** Item selection change.
	*/

	public void itemStateChanged(ItemEvent eEvent)
	{
		if (eEvent.getSource() == m_lbSource)
			onSelectLine();
	}

	/********************************************************************************
	** Remove the command parameter.
	*/

	private void onRemove()
	{
		Cmd oCmd = m_oLines.getCmd(m_lbSource.getSelectedIndex());

		oCmd.getParent().remove(oCmd);

		refreshListing();

		m_bModified = true;
	}

	/********************************************************************************
	** Edit the commands parameter.
	*/

	private void onEdit() throws InvalidParamException
	{
		if (m_ebParam.getText().length() == 0)
			throw new InvalidParamException("You must supply a value.", m_ebParam);

		Cmd    oCmd        = m_oLines.getCmd(m_lbSource.getSelectedIndex());
		String strOldParam = oCmd.getParameter();
		String strNewParam = m_ebParam.getText();

		if (!strNewParam.equals(strOldParam))
		{
			oCmd.setParameter(strNewParam);

			refreshListing();

			m_bModified = true;
		}
	}

	/********************************************************************************
	** Line selected in the source list.
	*/

	private void onSelectLine()
	{
		int		nSelection  = m_lbSource.getSelectedIndex();
		boolean	bEditable   = false;
		boolean	bRemoveable = false;
		String  strCmd      = "";
		String  strParam    = "";

		// Check if command takes parameters.
		if (nSelection >= 0)
		{
			Cmd oCmd    = m_oLines.getCmd(nSelection);
			bEditable   = oCmd.isParameterised();
			bRemoveable = oCmd.isRemoveable();

			strCmd   = m_oLines.getLine(nSelection);
			strParam = (!bEditable) ? "" : oCmd.getParameter();
		}

		// Set cmd and param text.
		m_ebCmd.setText(strCmd);
		m_ebParam.setText(strParam);

		// Enable/Disable value editing.
		m_ebParam.setEnabled(bEditable);
		m_btnRemove.setEnabled(bRemoveable);
		m_btnEdit.setEnabled(bEditable);
	}

	/********************************************************************************
	** Refresh the source listing.
	*/

	private void refreshListing()
	{
		int nSel = -1;

		// Save current selection for later.
		if (m_lbSource.getItemCount() > 0)
		{
			nSel = m_lbSource.getSelectedIndex();

			m_lbSource.removeAll();
		}

		// Fill the listbox with source code.
		m_oLines = m_oProgram.getSource();

		for(int i = 0; i < m_oLines.count(); i++)
			m_lbSource.add(m_oLines.getIndentedLine(i));

		// If first pass, select first line.
		if ( (nSel == -1) && (m_lbSource.getItemCount() > 0) )
			nSel = 0;

		// Old selection removed.?
		if (nSel >= m_lbSource.getItemCount())
			nSel--;

		// Restore selection.
		if (nSel >= 0)
			m_lbSource.select(nSel);

		onSelectLine();
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child Panels.
	private GroupPanel	m_pnlCmd	= new GroupPanel("Command & Value");
	private Panel		m_pnlEdit	= new Panel(new GridLayout(0, 1, 5, 5));
	private Panel		m_pnlBtns	= new Panel(new GridLayout(0, 1, 5, 5));

	// Child controls.
	private List		m_lbSource  = new List(25);
	private TextField	m_ebCmd     = new TextField(50);
	private Button		m_btnRemove = new Button("Remove");
	private TextField	m_ebParam   = new TextField(50);
	private Button		m_btnEdit   = new Button("Edit");

	// Data members.
	private Program		m_oProgram;
	private SourceLines	m_oLines;
	private boolean		m_bModified = false;
}
