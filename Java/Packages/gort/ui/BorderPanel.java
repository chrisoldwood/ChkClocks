package gort.ui;

import java.awt.*;

/********************************************************************************
** This panel derived class decorates a panel with a border.
*/

public class BorderPanel extends Panel
{
	/********************************************************************************
	** Default constructor.
	*/

	public BorderPanel()
	{
		this(NONE, CENTER, new Insets(5, 5, 5, 5), new BorderLayout());
	}

	/********************************************************************************
	** Constructor.
	*/

	public BorderPanel(LayoutManager lmManager)
	{
		this(NONE, CENTER, new Insets(5, 5, 5, 5), lmManager);
	}

	/********************************************************************************
	** Constructor.
	*/

	public BorderPanel(int nType, Insets oSize)
	{
		this(nType, CENTER, oSize, new BorderLayout());
	}

	/********************************************************************************
	** Constructor.
	*/

	public BorderPanel(int nType, Insets oSize, LayoutManager lmManager)
	{
		this(nType, CENTER, oSize, lmManager);
	}

	/********************************************************************************
	** Full constructor.
	*/

	public BorderPanel(int nType, int nAlignment, Insets oSize, LayoutManager lmManager)
	{
		super(lmManager);

		m_nType      = nType;
		m_nAlignment = nAlignment;
		m_oSize      = oSize;
	}

	/********************************************************************************
	** Returns the panels inner border dimensions.
	*/

	public Insets getInsets()
	{
		return m_oSize;
	}

	/********************************************************************************
	** Paint the control.
	** NB: Only handles ETCHED and CENTER at present.
	*/

	public void paint(Graphics g)
	{
		super.paint(g);

		Dimension dmSize = getSize();

		// Calculate box in centre?
		if (m_nAlignment == CENTER)
		{
			m_rcBorder.x      = (m_oSize.left / 2) - 1;
			m_rcBorder.y      = (m_oSize.top  / 2) - 1;
			m_rcBorder.width  = dmSize.width  - (m_rcBorder.x * 2) - 2;
			m_rcBorder.height = dmSize.height - m_rcBorder.y - ((m_oSize.bottom / 2) - 1) - 2;
		}

		// Draw an etched box?
		if (m_nType == ETCHED)
		{
			g.setColor(SystemColor.controlShadow);
			g.drawRect(m_rcBorder.x, m_rcBorder.y, m_rcBorder.width, m_rcBorder.height);

			g.setColor(SystemColor.controlLtHighlight);
			g.drawRect(m_rcBorder.x+1, m_rcBorder.y+1, m_rcBorder.width, m_rcBorder.height);
		}
	}

	/********************************************************************************
	** Constants
	*/

	// Border types.
	public static final int NONE    = 0;
	public static final int RAISED  = 1;
	public static final int LOWERED = 2;
	public static final int ETCHED  = 3;

	// Border alignment.
	public static final int OUTER  = 0;
	public static final int CENTER = 1;
	public static final int INNER  = 2;

	/********************************************************************************
	** Members.
	*/

	// Data members.
	private int			m_nType;
	private int			m_nAlignment;
	private Insets		m_oSize;

	// Graphics members.
	private Rectangle	m_rcBorder = new Rectangle();
}
