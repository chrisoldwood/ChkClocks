/********************************************************************************
** This class provides the algorithm for solving the puzzle.
*/

public class PuzzleSolver
{
	/********************************************************************************
	** Constructor. The parameters are the puzzle to solve and the return value is
	** is a 3D array of the possible answers for each square.
	*/

	public PuzzleSolver(SquareState[][] aoStates, boolean[][][] abSolution)
	{
		m_aoStates   = aoStates;
		m_abSolution = abSolution;

		// Initialise the solution.
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				if (aoStates[x][y].isAnswered())
					setAnswer(m_abWorkingSet, x, y, aoStates[x][y].getAnswer());
				else
					clearAnswer(m_abWorkingSet, x, y);
			}
		}
	}

	/********************************************************************************
	** Solve those squares that can be directly solved, i.e. are independent of any
	** other squares. This uses the following simple algorithmn:-
	**
	** 1: For each unanswered square, create a list of possible answers by removing
	**    the known answers from the row, column and block in which it resides.
	** 2: For each row, column and block, look for unanswered squares that contain
	**    an answer that no other square in the same row/column/block has.
	*/

	public void solveSquares()
	{
		// Process all squares without answers...
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				// Square contains no answer?
				if (!m_aoStates[x][y].isAnswered())
				{
					// Process row.
					processRange(x, y, 0, y, PUZZLE_SIZE, y+1);

					// Process column.
					processRange(x, y, x, 0, x+1, PUZZLE_SIZE);

					int r1 = (y / BLOCK_SIZE) * BLOCK_SIZE;
					int r2 = r1 + BLOCK_SIZE;
					int c1 = (x / BLOCK_SIZE) * BLOCK_SIZE;
					int c2 = c1 + BLOCK_SIZE;

					// Process 3x3 block.
					processRange(x, y, c1, r1, c2, r2);
				}
			}
		}

		// Create the initial solution.
		// NB: The methods below read from the WorkingSet but write to the
		// Solution so as not to generate new answers that are a result of
		// a deduction made earlier.
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				for (int p = 0; p < PUZZLE_SIZE; ++p)
				{
					m_abSolution[x][y][p] = m_abWorkingSet[x][y][p];
				}
			}
		}

		// Search all rows...
		for (int r = 0; r < PUZZLE_SIZE; ++r)
			searchRange(0, r, PUZZLE_SIZE, r+1);

		// Search all coloumns...
		for (int c = 0; c < PUZZLE_SIZE; ++c)
			searchRange(c, 0, c+1, PUZZLE_SIZE);

		// Search all 3x3 blocks...
		for (int i = 0; i < NUM_BLOCKS; ++i)
		{
			for (int j = 0; j < NUM_BLOCKS; ++j)
			{
				int r1 = i * BLOCK_SIZE;
				int r2 = r1 + BLOCK_SIZE;
				int c1 = j * BLOCK_SIZE;
				int c2 = c1 + BLOCK_SIZE;

				searchRange(c1, r1, c2, r2);
			}
		}
	}

	/********************************************************************************
	** Process the range (row, column or block) that contains (x, y) to reduce the
	** set of possible answers to those which are as yet unused.
	** The arguments represent the range [c1, c2) , [r1, r2).
	*/

	private void processRange(int x, int y, int c1, int r1, int c2, int r2)
	{
		// Sanity check the arguments.
		if ( ((r2-r1) * (c2-c1)) != PUZZLE_SIZE)
			throw new IllegalArgumentException("[" + c1 + "," + r1  + ")-[" + c2 + "," + r2 + ")");

		if ( (x < c1) || (x >= c2) || (y < r1) || (y >= r2) )
			throw new IllegalArgumentException("(" + x + "," + y + ") ->" + "[" + c1 + "," + r1  + "),[" + c2 + "," + r2 + ")");

		// For all squares in the range...
		for (int r = r1; r < r2; ++r)
		{
			for (int c = c1; c < c2; ++c)
			{
				// Ignore ourself.
				if ( (c != x) || (r != y) )
				{
					// Square contains an answer to remove?
					if (m_aoStates[c][r].isAnswered())
					{
						int p = m_aoStates[c][r].getAnswer()-1;

						m_abWorkingSet[x][y][p] = false;
					}
				}
			}
		}
	}

	/********************************************************************************
	** Search the range (row, column or block) for unique answers.
	** The arguments represent the range [c1, c2) , [r1, r2).
	*/

	private void searchRange(int c1, int r1, int c2, int r2)
	{
		// Sanity check the arguments.
		if ( ((r2-r1) * (c2-c1)) != PUZZLE_SIZE)
			throw new IllegalArgumentException("[" + c1 + "," + r1  + ")-[" + c2 + "," + r2 + ")");

		int[] anCounts = new int[PUZZLE_SIZE];

		// For all squares in the range...
		for (int r = r1; r < r2; ++r)
		{
			for (int c = c1; c < c2; ++c)
			{
				// Square contains no answer yet?
				if (!m_aoStates[c][r].isAnswered())
				{
					// Update reference counts for each possible answer.
					for (int p = 0; p < PUZZLE_SIZE; ++p)
					{
						if (m_abWorkingSet[c][r][p])
							++anCounts[p];
					}
				}
			}
		}

		// Find any unique answers...
		for (int p = 0; p < PUZZLE_SIZE; ++p)
		{
			if (anCounts[p] == 1)
			{
				// Update the square with that answer.
				for (int r = r1; r < r2; ++r)
				{
					for (int c = c1; c < c2; ++c)
					{
						if (m_abWorkingSet[c][r][p])
							setAnswer(m_abSolution, c, r, p+1);
					}
				}
			}
		}
	}

	/********************************************************************************
	** Helper method to get the answer for a single square, if it exists. If there
	** is no single answer it returns -1.
	*/

	public static int getAnswer(boolean[][][] abSolution, int x, int y)
	{
		int nAnswer = -1;

		// For all square answers...
		for (int p = 0; p < PUZZLE_SIZE; ++p)
		{
			if (abSolution[x][y][p])
			{
				// No unique answer?
				if (nAnswer != -1)
					return -1;

				nAnswer = p+1;
			}
		}

		return nAnswer;
	}

	/********************************************************************************
	** Helper method to set the answer for a single square.
	*/

	public static void setAnswer(boolean[][][] abSolution, int x, int y, int nAnswer)
	{
		for (int p = 0; p < PUZZLE_SIZE; ++p)
		{
			abSolution[x][y][p] = ((p+1) == nAnswer);
		}
	}

	/********************************************************************************
	** Helper method to clear the answer for a single square.
	*/

	public static void clearAnswer(boolean[][][] abSolution, int x, int y)
	{
		for (int p = 0; p < PUZZLE_SIZE; ++p)
		{
			abSolution[x][y][p] = true;
		}
	}

	/********************************************************************************
	** Contants.
	*/

	private static final int BLOCK_SIZE	 = PuzzleGrid.BLOCK_SIZE;
	private static final int PUZZLE_SIZE = PuzzleGrid.PUZZLE_SIZE;
	private static final int NUM_BLOCKS  = PUZZLE_SIZE / BLOCK_SIZE;
	

	/********************************************************************************
	** Members.
	*/

	private SquareState[][]	m_aoStates     = null;
	private boolean[][][]	m_abWorkingSet = new boolean[PUZZLE_SIZE][PUZZLE_SIZE][PUZZLE_SIZE];
	private boolean[][][]	m_abSolution   = null;
}
