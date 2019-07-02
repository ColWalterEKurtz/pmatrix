// -----------------------------------------------------------------------------
// LineReader.cpp                                                 LineReader.cpp
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the implementation of the @ref LineReader class.
 * @author     Col. Walter E. Kurtz
 * @version    2019-07-02
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include "LineReader.h"


// -----------------------------------------------------------------------------
// Used namespaces                                               Used namespaces
// -----------------------------------------------------------------------------
using namespace std;


// -----------------------------------------------------------------------------
// Construction                                                     Construction
// -----------------------------------------------------------------------------

// ----------
// LineReader
// ----------
/*
 *
 */
LineReader::LineReader(istream& src, int maxBytes)
: m_src(src)
{
  m_maxBytes = maxBytes;
  m_recent   = 0;
  m_line     = "";
}


// -----------------------------------------------------------------------------
// Handling                                                             Handling
// -----------------------------------------------------------------------------

// --------
// readLine
// --------
/*
 *
 */
int LineReader::readLine()
{
  // check limit
  if (m_maxBytes < 1)
  {
    // unable to read anything
    return 0;
  }

  // reset global buffer
  m_line = "";

  // number of extracted bytes
  int xcount = 0;

  // number of buffered bytes
  int bcount = 0;

  // continue long line
  if ( (m_recent !=  0)
  &&   (m_recent != 10)
  &&   (m_recent != 13) )
  {
    m_line += m_recent;
    xcount += 1;
    bcount += 1;
  }

  // one byte from the input stream
  char c = 0;

  // get next character from input stream
  while ( m_src.get(c) )
  {
    // CR extracted
    if (c == 13)
    {
      // update counter
      xcount += 1;

      // update recent character
      m_recent = c;

      // line finished
      break;
    }

    // LF extracted
    else if (c == 10)
    {
      // don't trigger [CR][LF] twice
      if (m_recent != 13)
      {
        // update counter
        xcount += 1;

        // update recent character
        m_recent = c;

        // line finished
        break;
      }

      // just update recent character
      m_recent = c;
    }

    // plain character extracted
    else
    {
      // update recent character
      m_recent = c;

      // update counter
      xcount += 1;
      bcount += 1;

      // check limit
      if (bcount > m_maxBytes)
      {
        // signalize long line
        return 2;
      }

      // append plain character
      m_line += c;
    }
  }

  // check number of extracted bytes
  return (xcount > 0) ? 1 : 0;
}

// -------
// getLine
// -------
/*
 *
 */
string LineReader::getLine() const
{
  return m_line;
}

