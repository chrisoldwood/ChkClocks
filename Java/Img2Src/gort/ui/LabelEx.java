import java.awt.*;
import java.util.*;

/********************************************************************************
** This is an enhanced Label class that supports multiple lines.
*/

public class LabelEx extends Canvas
{
	/********************************************************************************
	** Default constructor.
	*/

	public LabelEx()
	{
	}

	/********************************************************************************
	** Full constructor.
	*/

	public LabelEx(String strLabel)
	{
		setText(strLabel);
	}

	/********************************************************************************
	** Set the labels text.
	** NB: Can't use StringTokenizer as it counts \n\n as a single delimiter rather
	**     than returning an empty token.
	*/

	public void setText(String strLabel)
	{
		// Force dimension recalc.
		m_nMaxWidth   = -1;
		m_nLineHeight = -1;

		// Split the text into lines.
		StrTok st = new StrTok();
		m_astrLines = st.splitLines(strLabel);
	}

	/********************************************************************************
	** Paint the component.
	*/

	public void paint(Graphics g)
	{
		// Text supplied?
		if (m_astrLines != null)
		{
			// Get the component size.
			Dimension dmSize  = getSize();
			Insets    oInsets = getInsets();

			// Calculate the base offsets.
			int nXOffset = oInsets.left;
			int nYOffset = (dmSize.height - (m_astrLines.length * m_nLineHeight)) / 2;

			// Get the font metrics.
			FontMetrics oMetrics = g.getFontMetrics();

			// Adjust Y offset to baseline.
			nYOffset += oMetrics.getLeading() + oMetrics.getAscent();

			g.setColor(SystemColor.controlText);

			for (int i = 0, y = nYOffset; i < m_astrLines.length; i++)
			{
				g.drawString(m_astrLines[i], nXOffset, y);
				y += m_nLineHeight;
			}
		}
	}

	/********************************************************************************
	** Gets the dimensions of the component.
	*/

	public Dimension getPreferredSize()
	{
		// Calculate dimensions?
		if ( (m_nMaxWidth == -1) || (m_nLineHeight == -1) )
		{
			FontMetrics oMetrics = getGraphics().getFontMetrics();

			m_nMaxWidth   = 0;
			m_nLineHeight = oMetrics.getHeight();

			// Find the longest line.
			for (int i = 0; i < m_astrLines.length; i++)
				m_nMaxWidth = Math.max(oMetrics.stringWidth(m_astrLines[i]), m_nMaxWidth);
		}

		Insets oInsets = getInsets();

		return new Dimension(m_nMaxWidth + oInsets.left + oInsets.right,
							 (m_astrLines.length * m_nLineHeight) + oInsets.top + oInsets.bottom);
	}

	public Dimension getMinimumSize()
	{
		return getPreferredSize();
	}

	/********************************************************************************
	** Gets the insets of the component.
	*/

	public Insets getInsets()
	{
		return s_oInsets;
	}

	/********************************************************************************
	** Constants.
	*/

	// Label alignment.
	public static final int LEFT   = Label.LEFT;
	public static final int CENTER = Label.CENTER;
	public static final int RIGHT  = Label.RIGHT;

	// Default insets.
	public static final Insets s_oInsets = new Insets(5, 5, 5, 5);

	/********************************************************************************
	** Members.
	*/

	private String[]	m_astrLines;
	private int			m_nMaxWidth   = -1;
	private int			m_nLineHeight = -1;
}
