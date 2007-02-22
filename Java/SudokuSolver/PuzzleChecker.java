/********************************************************************************
** This class provides the algorithm for checking the puzzle for errors.
*/

public class PuzzleChecker
{
	/********************************************************************************
	** Constructor. The parameters are the puzzle the check and an equivalent 2D
	** array for reporting errors.
	*/

	public PuzzleChecker(PuzzleSquare[][] aoSquares, boolean[][] abErrors)
	{
		m_aoSquares = aoSquares;
		m_abErrors  = abErrors;
	}

	/********************************************************************************
	** Check that both the predefined and user supplied answers are valid. This
	** modifies the errors array directly and returns the number of errors found.
	*/

	public int check()
	{
		// Check all squares...
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				PuzzleSquare oSquare = m_aoSquares[x][y];
				int          nState  = oSquare.getState().getState();

				// Square contains an answer to validate?
				if ( (nState == SquareState.PREDEFINED_ANSWER)
				  || (nState == SquareState.USER_ANSWER) )
				{
					checkRow(x, y);
					checkColumn(x, y);
					checkBlock(x, y);
				}
			}
		}

		int nErrors = 0;

		// Count errors...
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				if (m_abErrors[x][y])
					++nErrors;
			}
		}

		return nErrors;
	}

	/********************************************************************************
	** Checks the row that contains (x,y) for any errors.
	*/

	private void checkRow(int x, int y)
	{
		PuzzleSquare oSquare = m_aoSquares[x][y];
		int          nAnswer = oSquare.getState().getAnswer();

		for (int c = 0; c < PUZZLE_SIZE; ++c)
		{
			// Ignore ourself.
			if (c != x)
			{
				PuzzleSquare oOtherSquare = m_aoSquares[c][y];
				int          nOtherState  = oOtherSquare.getState().getState();
								
				// Square contains an answer to compare with?
				if ( (nOtherState == SquareState.PREDEFINED_ANSWER)
				  || (nOtherState == SquareState.USER_ANSWER) )
				{
					int nOtherAnswer = oOtherSquare.getState().getAnswer();

					if (nAnswer == nOtherAnswer)
					{
						m_abErrors[x][y] = true;
						m_abErrors[c][y] = true;
					}
				}
			}
		}
	}

	/********************************************************************************
	** Checks the column that contains (x,y) for any errors.
	*/

	private void checkColumn(int x, int y)
	{
		PuzzleSquare oSquare = m_aoSquares[x][y];
		int          nAnswer = oSquare.getState().getAnswer();

		for (int r = 0; r < PUZZLE_SIZE; ++r)
		{
			// Ignore ourself.
			if (r != y)
			{
				PuzzleSquare oOtherSquare = m_aoSquares[x][r];
				int          nOtherState  = oOtherSquare.getState().getState();
								
				// Square contains an answer to compare with?
				if ( (nOtherState == SquareState.PREDEFINED_ANSWER)
				  || (nOtherState == SquareState.USER_ANSWER) )
				{
					int nOtherAnswer = oOtherSquare.getState().getAnswer();

					if (nAnswer == nOtherAnswer)
					{
						m_abErrors[x][y] = true;
						m_abErrors[x][r] = true;
					}
				}
			}
		}
	}

	/********************************************************************************
	** Checks the 3x3 block that contains (x,y) for any errors.
	*/

	private void checkBlock(int x, int y)
	{
		PuzzleSquare oSquare = m_aoSquares[x][y];
		int          nAnswer = oSquare.getState().getAnswer();
		int          r1      = (y/3)*3;
		int          r2      = r1 + 3;
		int          c1      = (x/3)*3;
		int          c2      = c1 + 3;

		// For all squares in the 3x3 block...
		for (int r = r1; r < r2; ++r)
		{
			for (int c = c1; c < c2; ++c)
			{
				// Ignore ourself.
				if ( (c != x) && (r != y) )
				{
					PuzzleSquare oOtherSquare = m_aoSquares[c][r];
					int          nOtherState  = oOtherSquare.getState().getState();
									
					// Square contains an answer to compare with?
					if ( (nOtherState == SquareState.PREDEFINED_ANSWER)
					  || (nOtherState == SquareState.USER_ANSWER) )
					{
						int nOtherAnswer = oOtherSquare.getState().getAnswer();

						if (nAnswer == nOtherAnswer)
						{
							m_abErrors[x][y] = true;
							m_abErrors[c][r] = true;
						}
					}
				}
			}
		}
	}

	/********************************************************************************
	** Contants.
	*/

	private static final int PUZZLE_SIZE = PuzzleGrid.PUZZLE_SIZE;

	/********************************************************************************
	** Members.
	*/

	// Data members.
	private PuzzleSquare[][]	m_aoSquares = null;
	private boolean[][]			m_abErrors  = null;
}
