// -----------------------------------------------------------------------------
// main.cpp                                                             main.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the program's main() function
 *             and doxygen's main page.
 * @author     Col. Walter E. Kurtz
 * @version    2019-07-03
 * @copyright  GNU General Public License - Version 3.0
 *
 * @mainpage
 *
 * @section issue What is the issue here?
 *   This filter creates the LaTeX code of a pmatrix-environment
 *   from given entries (one per line).
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include "LineReader.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Functions                                                           Functions
// -----------------------------------------------------------------------------

// -----------
// getUTF8size
// -----------
/**
 * @brief  This function tries to count the number of UTF-8 encoded characters.
 */
unsigned getUTF8size(const string& text)
{
  // UTF-8 flags
  static const char mask = (-64);  // 11000000
  static const char next = (-128); // 10000000

  // number of encoded characters
  unsigned n = 0;

  // count characters
  for(string::size_type i = 0; i < text.size(); i++)
  {
    // get character
    const char& c = text[i];

    // check two highest bits
    if ((c & mask) != next)
    {
      // increase counter
      n += 1;
    }
  }

  // return number of encoded characters
  return n;
}

// ---------
// getIndent
// ---------
/**
 * @brief  This method returns all leading horizontal whitespace characters.
 */
string getIndent(const string& line)
{
  string indent("");

  // check each character
  for(string::size_type i = 0; i < line.size(); i++)
  {
    // get current character
    char c = line[i];

    // horizontal whitespace
    if ( (c == 9) || (c == 32) )
    {
      indent += c;
    }

    // any other character
    else
    {
      break;
    }
  }

  return indent;
}

// ----
// trim
// ----
/**
 * @brief  This functions removes whitespace characters from
 *         the beginning and from the end of the given string.
 */
string trim(const string& line)
{
  // the trimmed string
  string trimmed("");

  // subsequent whitespace characters
  string buffer("");

  // check each character
  for(string::size_type i = 0; i < line.size(); i++)
  {
    // get current character
    char c = line[i];

    // whitespace characters
    if ( (c ==  9)
    ||   (c == 10)
    ||   (c == 13)
    ||   (c == 32) )
    {
      // skip leading whitespace characters
      if ( !trimmed.empty() )
      {
        buffer += c;
      }
    }

    // visible character found
    else
    {
      // check buffer first
      if ( !buffer.empty() )
      {
        // add buffered characters first
        trimmed.append(buffer);

        // reset buffer
        buffer = "";
      }

      // append visible characer
      trimmed += c;
    }
  }

  return trimmed;
}

// ----------
// showMatrix
// ----------
/**
 * @brief  This function creates the LaTeX code of a matrix environment.
 */
void showMatrix( const string&          indent,
                 const string&          environment,
                 const vector<string>&  entries,
                 unsigned               nrows,
                 unsigned               ncols
               )
{
  // the width of each column
  vector<unsigned> colw(ncols);

  // find maximum column width
  for(unsigned j = 0; j < ncols; j++)
  {
    // set initial width
    colw[j] = 0;

    // down this column
    for(unsigned i = 0; i < nrows; i++)
    {
      // calculate linear index
      unsigned n = i * ncols + j;

      // get number of characters
      unsigned w = getUTF8size(entries[n]);

      // check maximum
      if (w > colw[j])
      {
        // update maximum width
        colw[j] = w;
      }
    }
  }

  // open environment
  cout << indent << "\\begin{" << environment << "}" << endl;
  cout << indent << "  ";

  // show entries
  for(unsigned i = 0; i < nrows; i++)
  {
    // start next row
    if (i > 0)
    {
      cout << " \\\\" << endl;
      cout << indent << "  ";
    }

    for(unsigned j = 0; j < ncols; j++)
    {
      // start next column
      if (j > 0)
      {
        cout << " & ";
      }

      // calculate linear index
      unsigned n = i * ncols + j;

      // show entry
      cout << setw(colw[j]) << right << entries[n];
    }
  }

  // close environment
  cout << endl;
  cout << indent << "\\end{" << environment << "}" << endl;
}

// ----
// main
// ----
/**
 * @brief  The program starts in this function.
 *
 * @param argc  holds the number of passed command-line arguments.
 * @param argv  holds the list of passed command-line arguments.
 *
 * @return
 * Value | Meaning
 * ----: | :------
 *     0 | The requested operation finished successfully.
 *     1 | The requested operation failed.
 */
int main(int argc, char** argv)
{
  // the size of the resulting matrix
  unsigned nrows = 0;
  unsigned ncols = 0;

  // the entries of the resulting matrix
  vector<string> entries;

  // set initial indent
  string indent = "";

  // use this object to read lines
  LineReader reader;

  // the reader's return value to analyze
  int check = 0;

  // read all lines from stdin
  while ( (check = reader.readLine()) )
  {
    // long line found
    if (check == 2)
    {
      // notify user
      cerr << "line exceeds maximum number of bytes" << endl;

      // signalize trouble
      return 1;
    }

    // get extracted line
    string line = reader.getLine();

    // get initial indent
    if ( entries.empty() )
    {
      indent = getIndent(line);
    }

    // get trimmed line
    line = trim(line);

    // skip empty lines
    if ( line.empty() ) continue;

    // end of first row manually given
    if (line == "...")
    {
      // check if number of columns is still unknown
      if (ncols == 0)
      {
        // first empty line finishes first row of the matrix
        ncols = entries.size();
      }
    }

    // add 'empty' entry
    else if (line == ".")
    {
      entries.push_back("");
    }

    // add entry
    else
    {
      entries.push_back(line);
    }

    // check number of entries
    if (entries.size() > 10000)
    {
      // notify user
      cerr << "too many entries given" << endl;

      // signalize trouble
      return 1;
    }
  }

  // no lines extracted
  if ( entries.empty() )
  {
    // notify user
    cerr << "no entries given" << endl;

    // signalize trouble
    return 1;
  }

  // number of columns still unknown
  if (ncols == 0)
  {
    // calculate square root
    int root = static_cast<int>( sqrt(entries.size()) );

    // check if quadratic matrix is given
    if ( (root * root) == entries.size() )
    {
      nrows = root;
      ncols = root;
    }

    // create vector by default
    else
    {
      ncols = 1;
      nrows = entries.size();
    }
  }

  // number of columns known
  else
  {
    // calculate number of rows
    nrows = entries.size() / ncols;

    // check matrix size
    if ( (nrows * ncols) != entries.size() )
    {
      // notify user
      cerr << "unable to determin matrix size (use . for missing entries)" << endl;

      // signalize trouble
      return 1;
    }
  }

  // create pmatrix
  showMatrix(indent, "pmatrix", entries, nrows, ncols);

  // signalize success
  return 0;
}

