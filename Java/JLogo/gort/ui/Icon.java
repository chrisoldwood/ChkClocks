package gort.ui;

import java.awt.*;

/********************************************************************************
** This canvas derived class is used to display an icon image.
*/

public class Icon extends Canvas
{
	/********************************************************************************
	** Constructor.
	*/

	public Icon()
	{
		setBackground(SystemColor.control);
	}

	/********************************************************************************
	** Set the image to display.
	*/

	public void setImage(Image oImage)
	{
		m_imgIcon  = oImage;
		m_rcBounds = new Rectangle(0, 0, DEF_WIDTH, DEF_HEIGHT);

		if (m_imgIcon != null)
		{
			m_rcBounds.width  = m_imgIcon.getWidth(null);
			m_rcBounds.height = m_imgIcon.getHeight(null);
		}

		repaint();
	}

	public void setImage(Image oImage, Rectangle rcBounds)
	{
		m_imgIcon  = oImage;
		m_rcBounds = rcBounds.getBounds();

		repaint();
	}

	/********************************************************************************
	** Get the preferred size.
	*/

	public Dimension getPreferredSize()
	{
		return m_rcBounds.getSize();
	}

	/********************************************************************************
	** Paint the control.
	*/

	public void paint(Graphics g)
	{
		Dimension dmSize = getSize();

		int nXOffset = (dmSize.width  - m_rcBounds.width ) / 2;
		int nYOffset = (dmSize.height - m_rcBounds.height) / 2;

		if (m_imgIcon != null)
		{
			g.drawImage(m_imgIcon, nXOffset, nYOffset,
						nXOffset+m_rcBounds.width, nYOffset+m_rcBounds.height,
						m_rcBounds.x, m_rcBounds.y,
						m_rcBounds.x+m_rcBounds.width,
						m_rcBounds.y+m_rcBounds.height, this);
		}
	}

	/********************************************************************************
	** Constants.
	*/

	// Default icon dimesions.
	private static final int DEF_WIDTH  = 32;
	private static final int DEF_HEIGHT = 32;

	/********************************************************************************
	** Members.
	*/

	private Image		m_imgIcon;
	private Rectangle	m_rcBounds = new Rectangle(0, 0, DEF_WIDTH, DEF_HEIGHT);
}
