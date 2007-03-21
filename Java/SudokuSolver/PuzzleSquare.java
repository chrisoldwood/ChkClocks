import java.awt.*;
import java.text.*;

/********************************************************************************
** This is a square on the puzzle grid. It is a fully owner drawn control.
*/

public class PuzzleSquare extends Canvas
{
	/********************************************************************************
	** Constructor.
	*/

	public PuzzleSquare(PuzzleGrid oPuzzle, Point ptPosition)
	{
		// Initialise members.
		m_oPuzzle    = oPuzzle;
		m_ptPosition = ptPosition;
		m_bSelected  = false;;

		// Initialise UI.
		setBackground(NORMAL_BG_CLR);
	}

	/********************************************************************************
	** Gets the position of the square in the grid.
	*/

	public Point getPosition()
	{
		return m_ptPosition;
	}

	/********************************************************************************
	** Change the "selection" status of the square. If we are the new selection we
	** grab the keyboard focus.
	*/

	public void setSelection(boolean bSelected)
	{
		if (m_bSelected != bSelected)
		{
			m_bSelected = bSelected;

			if (m_bSelected)
				requestFocus();

			repaint();
		}
	}

	/********************************************************************************
	** Query if the square is the selected one.
	*/

	public boolean isSelected()
	{
		return m_bSelected;
	}

	/********************************************************************************
	** Gets the current state.
	*/

	public SquareState getState()
	{
		return m_oState;
	}

	/********************************************************************************
	** Sets a new state.
	*/

	public void setState(SquareState oState)
	{
		// Update members.
		m_oState = oState;

		// Update UI.
		if (m_oState.isPredefinedValue())
			setBackground(FIXED_BG_CLR);
		else
			setBackground(NORMAL_BG_CLR);

		repaint();
	}

	/********************************************************************************
	** Set the answer. The answer can either be one provided by the puzzle or one
	** provided by the user.
	*/

	public void setAnswer(int nAnswer, boolean bPredefined)
	{
		// Forward to state class.
		m_oState.setAnswer(nAnswer, bPredefined);

		// Update UI.
		if (bPredefined)
			setBackground(FIXED_BG_CLR);
		else
			setBackground(NORMAL_BG_CLR);

		repaint();
	}

	/********************************************************************************
	** Set the array of possible answers.
	*/

	public void setChoices(boolean[] abChoices)
	{
		// Forward to state class.
		m_oState.setChoices(abChoices);

		// Update UI.
		setBackground(NORMAL_BG_CLR);

		repaint();
	}

	/********************************************************************************
	** Set the style of the square. The style is used to denote posisble errors and
	** hints etc.
	*/

	public void setStyle(int nStyle)
	{
		m_nStyle = nStyle;

		// Update UI.
		if (nStyle == NORMAL_STYLE)
		{
			if (m_oState.isPredefinedValue())
				setBackground(FIXED_BG_CLR);
			else
				setBackground(NORMAL_BG_CLR);
		}
		else if (nStyle == ERROR_STYLE)
		{
			setBackground(ERROR_BG_CLR);
		}
		else if (nStyle == HINT_STYLE)
		{
			setBackground(HINT_BG_CLR);
		}

		repaint();
	}

	/********************************************************************************
	** Clear the square. The flag signals whether only user provided answers or all
	** values (i.e. predefined as well) should be cleared.
	*/

	public void clear(boolean bUserDataOnly)
	{
		// Only clear user values AND not a user supplied value?
		if (bUserDataOnly && m_oState.isPredefinedValue())
			return;

		// Forward to state class.
		m_oState.clear();

		// Update UI.
		setStyle(NORMAL_STYLE);

		repaint();
	}

	/********************************************************************************
	** Paint the control.
	*/

	public void paint(Graphics g)
	{
		Rectangle rcRect = new Rectangle(getSize());

		// At startup only draw the outer edges.
		if (m_oPuzzle.getUserState() == PuzzleGrid.INITIALISED)
		{
			g.setColor(OUTER_BORDER_CLR);

			if (m_ptPosition.x == 0)
				g.drawLine(rcRect.x, rcRect.y, rcRect.x, rcRect.y+rcRect.height-1);

			if (m_ptPosition.x == PuzzleGrid.PUZZLE_SIZE-1)
				g.drawLine(rcRect.x+rcRect.width-1, rcRect.y, rcRect.x+rcRect.width-1, rcRect.y+rcRect.height-1);

			if (m_ptPosition.y == 0)
				g.drawLine(rcRect.x, rcRect.y, rcRect.x+rcRect.width-1, rcRect.y);

			if (m_ptPosition.y == PuzzleGrid.PUZZLE_SIZE-1)
				g.drawLine(rcRect.x, rcRect.y+rcRect.height-1, rcRect.x+rcRect.width-1, rcRect.y+rcRect.height-1);

			return;
		}
		
		// Draw left border
		if ((m_ptPosition.x % 3) == 0)
			g.setColor(OUTER_BORDER_CLR);
		else
			g.setColor(INNER_BORDER_CLR);

		g.drawLine(rcRect.x, rcRect.y, rcRect.x, rcRect.y+rcRect.height-1);
	
		// Draw right border?
		if (m_ptPosition.x == PuzzleGrid.PUZZLE_SIZE-1)
		{
			g.setColor(OUTER_BORDER_CLR);
			g.drawLine(rcRect.x+rcRect.width-1, rcRect.y, rcRect.x+rcRect.width-1, rcRect.y+rcRect.height-1);
		}

		// Draw top border
		if ((m_ptPosition.y % 3) == 0)
			g.setColor(OUTER_BORDER_CLR);
		else
			g.setColor(INNER_BORDER_CLR);

		g.drawLine(rcRect.x, rcRect.y, rcRect.x+rcRect.width-1, rcRect.y);

		// Draw bottom border?
		if (m_ptPosition.y == PuzzleGrid.PUZZLE_SIZE-1)
		{
			g.setColor(OUTER_BORDER_CLR);
			g.drawLine(rcRect.x, rcRect.y+rcRect.height-1, rcRect.x+rcRect.width-1, rcRect.y+rcRect.height-1);
		}

		// Draw focus, if selected.
		if (m_bSelected)
		{
			for (int i = 0; i < FOCUS_WIDTH; ++i)
			{
				g.setColor(FOCUS_BORDER_CLR);
				g.drawRect(rcRect.x, rcRect.y, rcRect.width-1, rcRect.height-1);

				rcRect.grow(-1, -1);
			}
		}
		else
		{
			// Always allow for the focus rectangle.
			rcRect.grow(-FOCUS_WIDTH, -FOCUS_WIDTH);
		}

		int nState = m_oState.getState();

		// Draw the puzzle answer, if set.
		if ( (nState == SquareState.PREDEFINED_ANSWER)
		  || (nState == SquareState.USER_ANSWER) )
		{
			// Prepare for painting.
			int         nAnswer   = m_oState.getAnswer();
			String      strNumber = NumberFormat.getInstance().format(nAnswer);
			Font        oBigFont  = m_oPuzzle.getBigFont();
			FontMetrics oMetrics  = g.getFontMetrics(oBigFont);
			int         nWidth    = oMetrics.stringWidth("0");
			int         nHeight   = oMetrics.getAscent();
			int         nBaseline = nHeight;
			int         nX        = rcRect.x + ((rcRect.width  - nWidth ) / 2);
			int         nY        = rcRect.y + ((rcRect.height - nHeight) / 2) + nBaseline;

			// Paint the answer.
			if (nState == SquareState.PREDEFINED_ANSWER)
				g.setColor(FIXED_TEXT_CLR);

			if (nState == SquareState.USER_ANSWER)
				g.setColor(USER_TEXT_CLR);

			g.setFont(oBigFont);
			g.drawString(strNumber, nX, nY);
		}
		else if (nState == SquareState.USER_CHOICES)
		{
			// Prepare for painting.
			boolean[]   abChoices  = m_oState.getChoices();
			Font        oSmallFont = m_oPuzzle.getSmallFont();
			FontMetrics oMetrics   = g.getFontMetrics(oSmallFont);
			int         nWidth     = oMetrics.stringWidth("0");
			int         nHeight    = oMetrics.getAscent();
			int         nBaseline  = nHeight;
			int         nXBorder   = (rcRect.width  - (nWidth  * 3)) / 4;
			int         nYBorder   = (rcRect.height - (nHeight * 3)) / 4;

			// Paint the choices.
			g.setColor(USER_TEXT_CLR);
			g.setFont(oSmallFont);

			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					int nIndex = (y * 3) + x;

					if (abChoices[nIndex])
					{
						String strNumber = NumberFormat.getInstance().format(nIndex+1);
						int    nX        = rcRect.x + (nWidth  * x) + (nXBorder * (x + 1));
						int    nY        = rcRect.y + (nHeight * y) + (nYBorder * (y + 1)) + nBaseline;

						g.drawString(strNumber, nX, nY);
					}
				}
			}
		}
		else if (nState == SquareState.NO_ANSWER)
		{
		}
	}

	/********************************************************************************
	** Constants.
	*/

	private static final int FOCUS_WIDTH	= 3;

	public static final int NORMAL_STYLE	= 1;
	public static final int ERROR_STYLE		= 2;
	public static final int HINT_STYLE		= 3;

	private static final Color NORMAL_BG_CLR = Color.white;
	private static final Color FIXED_BG_CLR  = Color.white; /*lightGray;*/
	private static final Color ERROR_BG_CLR  = Color.red;
	private static final Color HINT_BG_CLR   = Color.green;

	private static final Color OUTER_BORDER_CLR = Color.black;
	private static final Color INNER_BORDER_CLR = Color.lightGray;
	private static final Color FOCUS_BORDER_CLR = Color.black;

	private static final Color FIXED_TEXT_CLR = Color.black;
	private static final Color USER_TEXT_CLR  = Color.blue;

	/********************************************************************************
	** Members.
	*/

	private PuzzleGrid	m_oPuzzle    = null;
	private Point		m_ptPosition = null;
	private boolean		m_bSelected  = false;
	private SquareState	m_oState     = new SquareState();
	private int			m_nStyle     = NORMAL_STYLE;
}
