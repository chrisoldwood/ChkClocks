import java.awt.*;

/********************************************************************************
** This border panel derived class decorates a panel with a label like the group
** box control in Windows.
*/

public class GroupPanel extends BorderPanel
{
	/********************************************************************************
	** Default constructor.
	*/

	public GroupPanel()
	{
		this(null, Label.CENTER, new BorderLayout());
	}

	/********************************************************************************
	** Full constructor.
	*/

	public GroupPanel(String strLabel, int nAlignment, LayoutManager lmManager)
	{
		super(ETCHED, new Insets(20, 10, 10, 10), lmManager);

		m_strLabel   = strLabel;
		m_nAlignment = nAlignment;
	}

	/********************************************************************************
	** Sets the panels label.
	*/

	public void setLabel(String str)
	{
		setLabel(str, m_nAlignment);
	}

	/********************************************************************************
	** Sets the panels label and alignment.
	*/

	public void setLabel(String str, int nAlignment)
	{
		m_strLabel   = str;
		m_nAlignment = nAlignment;

		m_rcLabel    = null;
		repaint();
	}

	/********************************************************************************
	** Paint the control.
	** NB: Only handles CENTER alignment at present.
	*/

	public void paint(Graphics g)
	{
		super.paint(g);

		// Get the container/font dimensions.
		Dimension	dmSize   = getSize();
		FontMetrics oMetrics = g.getFontMetrics();
		
		// Calculate the label rect.
		m_rcLabel.width  = oMetrics.stringWidth(m_strLabel);
		m_rcLabel.height = getInsets().top;
		m_rcLabel.x      = (dmSize.width - m_rcLabel.width) / 2;
		m_rcLabel.y      = 0;

		// Draw the label.
		g.clearRect(m_rcLabel.x-LEFT_MARGIN, m_rcLabel.y, m_rcLabel.width+MARGINS, m_rcLabel.height);
		g.setColor(Color.black);
		g.drawString(m_strLabel, m_rcLabel.x, m_rcLabel.y+oMetrics.getHeight());
	}

	/********************************************************************************
	** Constants
	*/

	// Label alignment.
	public static final int LEFT   = Label.LEFT;
	public static final int CENTER = Label.CENTER;
	public static final int RIGHT  = Label.RIGHT;

	// Label margins.
	public static final int LEFT_MARGIN  = 5;
	public static final int RIGHT_MARGIN = 5;
	public static final int MARGINS      = (LEFT_MARGIN + RIGHT_MARGIN);

	/********************************************************************************
	** Members.
	*/

	// Data members.
	private String		m_strLabel   = "";
	private int			m_nAlignment = Label.CENTER;

	// Graphics members.
	private Rectangle	m_rcLabel    = new Rectangle();
}
