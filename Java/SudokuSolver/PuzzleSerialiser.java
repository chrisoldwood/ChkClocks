import java.util.*;

/********************************************************************************
** This class is used to serialise and deserialise the puzzle to/from a text
** stream. The format is basically a CSV style text blob. Each 'value' can be,
**
** A number prefixed by '=' is a user supplied answer.
** A number prefixed by '$' is a predefined answer.
** A '?' followed by a series of numbers is a list of user supplied alternatives.
** A # if the square is empty.
*/

public class PuzzleSerialiser
{
	/********************************************************************************
	** Serialise the puzzle state to a string.
	*/

	public static String write(PuzzleSquare[][] aoSquares)
	{
		// Initial size assumes a number in each square.
		StringBuffer str = new StringBuffer((PUZZLE_SIZE * PUZZLE_SIZE) * 2);

		// For all squares...
		for (int y = 0; y < PUZZLE_SIZE; ++y)
		{
			for (int x = 0; x < PUZZLE_SIZE; ++x)
			{
				// Write the separator.
				if (x != 0)
					str.append(',');

				SquareState  oState  = aoSquares[x][y].getState();
				int          nState  = oState.getState();

				// Write the predefined answer, if set.
				if (nState == SquareState.PREDEFINED_ANSWER)
				{
					str.append('$');
					str.append(oState.getAnswer());
				}
				// Write the user answer, if set.
				else if (nState == SquareState.USER_ANSWER)
				{
					str.append('=');
					str.append(oState.getAnswer());
				}
				// Write the users alternatives, if set.
				else if (nState == SquareState.USER_CHOICES)
				{
					str.append('?');

					boolean[] abChoices = oState.getChoices();

					for (int i = 0; i < PUZZLE_SIZE; ++i)
					{
						if (abChoices[i])
							str.append(i+1);
					}
				}
				// Write an empty square.
				else
				{
					str.append('#');
				}
			}

			str.append("\n");
		}

		return str.toString();
	}

	/********************************************************************************
	** Deserialise the puzzle state from a string.
	*/

	public static SquareState[][] read(String str)
	{
		SquareState[][] aoStates = new SquareState[PUZZLE_SIZE][PUZZLE_SIZE];

		// Split the input into lines...
		StringTokenizer oLineTok = new StringTokenizer(str, "\r\n", false);

		int y = 0;

		while ( (oLineTok.hasMoreTokens()) && (y < PUZZLE_SIZE) )
		{
			String strLine = oLineTok.nextToken();

			// Split the line into fields.
			StringTokenizer oFieldTok = new StringTokenizer(strLine, ",", false);

			if (oFieldTok.countTokens() > 0)
			{
				int x = 0;

				// Parse each field into a square state.
				while ( (oFieldTok.hasMoreTokens()) && (x < PUZZLE_SIZE) )
				{
					String strField = oFieldTok.nextToken();

					// Is a predefined answer?
					if ( (strField.length() == 2) && (strField.charAt(0) == '$') )
					{
						int nAnswer = strField.charAt(1) - '0';

						if ( (nAnswer >= 1) && (nAnswer <= 9) )
						{
							aoStates[x][y] = new SquareState();

							aoStates[x][y].setAnswer(nAnswer, true);
						}
					}
					// Is a user supplied answer?
					else if ( (strField.length() == 2) && (strField.charAt(0) == '=') )
					{
						int nAnswer = strField.charAt(1) - '0';

						if ( (nAnswer >= 1) && (nAnswer <= 9) )
						{
							aoStates[x][y] = new SquareState();

							aoStates[x][y].setAnswer(nAnswer, false);
						}
					}
					// Is a set of user supplied alternatives?
					else if ( (strField.length() >= 2) && (strField.charAt(0) == '?') )
					{
						boolean[] abChoices = null;

						for (int i = 1; i < strField.length(); ++i)
						{
							int nAnswer = strField.charAt(i) - '0';

							if ( (nAnswer >= 1) && (nAnswer <= 9) )
							{
								if (abChoices == null)
									abChoices = new boolean[PUZZLE_SIZE];

								abChoices[nAnswer-1] = true;
							}
						}

						if (abChoices != null)
						{
							aoStates[x][y] = new SquareState();

							aoStates[x][y].setChoices(abChoices);
						}
					}
					
					++x;
				}

				++y;				
			}
		}

		return aoStates;
	}

	/********************************************************************************
	** Partially validate the text stream to check if it contains at least the right
	** number of rows and columns.
	*/

	public static boolean validate(String str)
	{
		int nSquares = 0;

		// Split the input into lines...
		StringTokenizer oLineTok = new StringTokenizer(str, "\r\n", false);

		while (oLineTok.hasMoreTokens())
		{
			String strLine = oLineTok.nextToken();

			// Split the line into fields.
			StringTokenizer oFieldTok = new StringTokenizer(strLine, ",", false);

			nSquares += oFieldTok.countTokens();
		}

		return (nSquares == (PUZZLE_SIZE * PUZZLE_SIZE));
	}

	/********************************************************************************
	** Contants.
	*/

	private static final int BLOCK_SIZE	 = PuzzleGrid.BLOCK_SIZE;
	private static final int PUZZLE_SIZE = PuzzleGrid.PUZZLE_SIZE;

	/********************************************************************************
	** Members.
	*/
}
