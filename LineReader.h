// -----------------------------------------------------------------------------
// LineReader.h                                                     LineReader.h
// -----------------------------------------------------------------------------
/**
 * @file
 * @brief      This file holds the definition of the @ref LineReader class.
 * @author     Col. Walter E. Kurtz
 * @version    2019-07-02
 * @copyright  GNU General Public License - Version 3.0
 */

// -----------------------------------------------------------------------------
// One-Definition-Rule                                       One-Definition-Rule
// -----------------------------------------------------------------------------
#ifndef LINEREADER_H_INCLUDE_NO1
#define LINEREADER_H_INCLUDE_NO1


// -----------------------------------------------------------------------------
// Includes                                                             Includes
// -----------------------------------------------------------------------------
#include <string>
#include <istream>
#include <iostream>


// ----------
// LineReader
// ----------
/**
 * @brief  This class reads lines from text streams.
 */
class LineReader
{

public:

  // ---------------------------------------------------------------------------
  // Construction                                                   Construction
  // ---------------------------------------------------------------------------

  // ----------
  // LineReader
  // ----------
  /**
   * @brief  The constructor sets the stream to read from and
   *         the maximum number of bytes per line.
   */
  LineReader(std::istream& src = std::cin, int maxBytes = 2048);


  // ---------------------------------------------------------------------------
  // Handling                                                           Handling
  // ---------------------------------------------------------------------------

  // --------
  // readLine
  // --------
  /**
   * @brief  This method tries to read one line from the input stream.
   *
   * @return
   * Value | Meaning
   * ----: | :------
   *     0 | nothing extracted
   *     1 | one complete line extracted
   *     2 | maximum number of bytes extracted, but line not yet finished
   */
  int readLine();

  // -------
  // getLine
  // -------
  /**
   * @brief  This method returns the line that has been extracted
   *         by the last call of readLine().
   */
  std::string getLine() const;


private:

  // ---------------------------------------------------------------------------
  // Attributes                                                       Attributes
  // ---------------------------------------------------------------------------

  /// the input stream to read from
  std::istream& m_src;

  /// the maximum number of bytes per line
  int m_maxBytes;

  /// the most recently extracted byte
  char m_recent;

  /// the line that has been extracted by the last call of readLine()
  std::string m_line;

};


#endif  /* #ifndef LINEREADER_H_INCLUDE_NO1 */

