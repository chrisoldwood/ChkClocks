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
				// Square contains an answer to validate?
				if (m_aoSquares[x][y].getState().isAnswered())
				{
					int r1 = (y / BLOCK_SIZE) * BLOCK_SIZE;
					int r2 = r1 + BLOCK_SIZE;
					int c1 = (x / BLOCK_SIZE) * BLOCK_SIZE;
					int c2 = c1 + BLOCK_SIZE;

					// Process 3x3 block.
					checkRange(x, y, c1, r1, c2, r2);
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
	** Checks the range (row, column or block) that contains (x,y) for any errors.
	** The arguments represent the range [c1, c2) , [r1, r2).
	*/

	private void checkRange(int x, int y, int c1, int r1, int c2, int r2)
	{
		// Sanity check the arguments.
		if ( ((r2-r1) * (c2-c1)) != PUZZLE_SIZE)
			throw new IllegalArgumentException("[" + c1 + "," + r1  + ")-[" + c2 + "," + r2 + ")");

		if ( (x < c1) || (x >= c2) || (y < r1) || (y >= r2) )
			throw new IllegalArgumentException("(" + x + "," + y + ") ->" + "[" + c1 + "," + r1  + "),[" + c2 + "," + r2 + ")");

		PuzzleSquare oSquare = m_aoSquares[x][y];
		int          nAnswer = oSquare.getState().getAnswer();

		// For all squares in the range...
		for (int r = r1; r < r2; ++r)
		{
			for (int c = c1; c < c2; ++c)
			{
				// Ignore ourself.
				if ( (c != x) || (r != y) )
				{
					PuzzleSquare oOtherSquare = m_aoSquares[c][r];
									
					// Square contains an answer to compare with?
					if (m_aoSquares[c][r].getState().isAnswered())
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

	private static final int BLOCK_SIZE	 = PuzzleGrid.BLOCK_SIZE;
	private static final int PUZZLE_SIZE = PuzzleGrid.PUZZLE_SIZE;

	/********************************************************************************
	** Members.
	*/

	// Data members.
	private PuzzleSquare[][]	m_aoSquares = null;
	private boolean[][]			m_abErrors  = null;
}
