import java.awt.*;
import java.awt.event.*;
import gort.ui.*;

/********************************************************************************
** This panel is used to host the display status controls.
*/

public class DisplayStatusPanel extends GroupPanel
{
	/********************************************************************************
	** Constructor.
	*/

	public DisplayStatusPanel(Display oDisplay)
	{
		super("Display Status", GroupPanel.CENTER, new BorderLayout(0, 5));

		// Save parameters.
		m_oDisplay = oDisplay;

		// Create this panel.
		setBackground(SystemColor.control);
		add(BorderLayout.CENTER, m_pnlFields);

		// Create the child panels.
		m_pnlFields.add(m_lblWidth,  m_ebWidth);
		m_pnlFields.add(m_lblHeight, m_ebHeight);
		m_pnlFields.add(m_lblDummy1, m_lblDummy2);

		// Make status fields read-only.
		m_ebWidth.setEditable(false);
		m_ebHeight.setEditable(false);

		// Add event handler.
		m_oDisplay.addComponentListener(new ResizeListener());
	}

	/********************************************************************************
	** Adaptor to handle component events.
	*/

	public class ResizeListener extends ComponentAdapter
	{
		public void componentResized(ComponentEvent eEvent)
		{
			Dimension dmSize = m_oDisplay.getDimensions();

			// Update the status controls.
			m_ebWidth.setText(String.valueOf(dmSize.width));
			m_ebHeight.setText(String.valueOf(dmSize.height));
		}
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private DualColPanel m_pnlFields  = new DualColPanel();

	// Child controls.
	private Label		m_lblWidth  = new Label("Width:", Label.RIGHT);
	private TextField	m_ebWidth   = new TextField(4);
	private Label		m_lblHeight = new Label("Height:", Label.RIGHT);
	private TextField	m_ebHeight  = new TextField(4);
	private Label		m_lblDummy1 = new Label("");
	private Label		m_lblDummy2 = new Label("");

	// Data members.
	private Display		m_oDisplay;
}
