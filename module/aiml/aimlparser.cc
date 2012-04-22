// aimlparser.cc
// 7/31/2011

#include "aimlparser.h"
#include "aiml_config.h"

#include <QtCore>
#include <QtXml>
#include <cstdlib>

#ifdef Q_WS_WIN
#  include <qt_windows.h>
#  include <io.h>
#  include <fcntl.h>
#endif

namespace { // anonymous

  inline void reset_srand()
  {
    QTime currentTime = QTime::currentTime();
    int val = currentTime.msec() + currentTime.second() + currentTime.minute();
    srand(val);
  }

} // namespace

namespace { // anonymous, regexp

  // A much faster replacement of regExp.exactMatch(str)
  // it also captures the words corresponding to the wildcards * & _
  bool
  exactMatch(const QString &regExp, const QString &str, QStringList &capturedText)
  {
    QStringList regExpWords = regExp.split(' ');
    QStringList strWords = str.split(' ');
    if ((!regExpWords.count() || !strWords.count()) && (regExpWords.count() != strWords.count()))
      return false;
    if (regExpWords.count() > strWords.count())
      return false;
    QStringList::ConstIterator regExpIt = regExpWords.begin();
    QStringList::ConstIterator strIt = strWords.begin();
    while (strIt != strWords.end() && regExpIt != regExpWords.end()) {
      if (*regExpIt == "*" || *regExpIt == "_") {
        regExpIt++;
        QStringList capturedStr;
        if (regExpIt != regExpWords.end()) {
          QString nextWord = *regExpIt;
          if (nextWord != "*" && nextWord != "_") {
            forever {
              if (*strIt == nextWord)
                break;
              capturedStr += *strIt;
              if (++strIt == strWords.end())
                return false;
            }
          } else {
            capturedStr += *strIt;
            -regExpIt;
          }
        } else {
          forever {
            capturedStr += *strIt;
            if (++strIt == strWords.end())
              break;
          }
          capturedText += capturedStr.join(" ");
          return true;
        }
        capturedText += capturedStr.join(" ");
      } else if (*regExpIt != *strIt)
        return false;
      ++regExpIt;
      ++strIt;
    }
    return regExpIt == regExpWords.end() && strIt == strWords.end();
  }

} // anonymous namespace

namespace { // anonymous, string

  QString normalized(const QString &str)
  {
    QString ret;
    for (int i = 0; i < str.length(); i++) {
      QChar c = str.at(i);
      if (c.isLetterOrNumber() || (c == '*') || (c == '_') || (c == ' '))
        ret += c.toLower();
    }
    return ret;
  }

} // anonymous

namespace { // anonymous, dom

  QList<QDomNode>
  elementsByTagName(const QDomNode *node, const QString &tagName)
  {
    Q_ASSERT(node);
    QList<QDomNode> ret;
    QDomNodeList childNodes = node->toElement().elementsByTagName(tagName);
    for (int i = 0; i < childNodes.count(); i++) {
      QDomNode n = childNodes.item(i);
      if (n.parentNode() == *node)
        ret.append(n);
    }
    return ret;
  }

} // anonymous namespace

// - Constructions -
AimlParser::AimlParser() { ::reset_srand(); }

// - Nodes and leaves -

bool
AimlParser::Node::match(QStringList::const_iterator input, const QStringList &inputWords,
  const QString &currentThat, const QString &currentTopic, QStringList &capturedThatTexts,
  QStringList &capturedTopicTexts, Leaf *&leaf)
{
  if (input == inputWords.end())
     return false;

  if ((word == "*") || (word == "_")) {
    while (++input != inputWords.end())
      foreach (Node *child, children)
        if (child->match(
              input, inputWords, currentThat, currentTopic,
              capturedThatTexts, capturedTopicTexts, leaf))
            return true;

  } else {
    if (!word.isEmpty()) {
      if (word != *input)
        return false;
      ++input;
    }
    foreach (Node *child, children)
      if (child->match(
            input, inputWords, currentThat, currentTopic,
            capturedThatTexts, capturedTopicTexts, leaf))
        return true;
  }
  if (input == inputWords.end()) {
    foreach (leaf, leaves) {
      capturedThatTexts.clear();
      capturedTopicTexts.clear();
      if ( (!leaf->that.isEmpty() && !exactMatch(leaf->that, currentThat, capturedThatTexts))
          || (!leaf->topic.isEmpty() && !exactMatch(leaf->topic, currentTopic, capturedTopicTexts)) )
        continue;
      return true;
    }
  }

  return false;
}

/*
void
AimlParser::Node::debug(QTextStream* logStream, int indent)
{
  QString indentStr = QString().fill('\t', indent);
  (*logStream) << indentStr << word << " :\n";
  foreach (Node *child, children)
    child->debug(logStream, indent + 1);
  indentStr = QString().fill('\t', indent + 1);
  foreach (Leaf *leaf, leaves)
    (*logStream) << indentStr << "<topic-" << leaf->topic << " that-" << leaf->that << ">\n";
}
*/

// - Parser -

/*
void
AimlParser::runRegression()
{
  QDomDocument doc;
  QFile file( "utils/TestSuite.xml" );
  if ( !file.open(QFile::ReadOnly) )
    return;
  if ( !doc.setContent( &file ) )
  {
    file.close();
    (*logStream) << "Error while parsing " << file.fileName() << "\n";
    return;
  }
  file.close();

  (*logStream) << "Regression running:\n";

  loadAiml("utils/TestSuite.aiml");

  QDomElement docElem = doc.documentElement();
  QDomNodeList testCaseList = docElem.elementsByTagName ("TestCase");
  for (int i = 0; i < testCaseList.count(); i++)
  {
    QDomElement n = testCaseList.item(i).toElement();
    QString description = n.namedItem("Description").firstChild().nodeValue();
    QString input = n.namedItem("Input").firstChild().nodeValue();

    QString expectedAnswer = "";
    QDomNode child = n.namedItem("ExpectedAnswer").firstChild();
    while (!child.isNull()) {
      if (child.isText())
         expectedAnswer += child.toText().nodeValue();
      child = child.nextSibling();
    }
    (*logStream) << "===========================================================================\n";
    (*logStream) << "::Description: " + description + "\n";
    (*logStream) << "::Expected answer: " + expectedAnswer + "\n";
    QString answer = getResponse(input);
    if (answer.simplified().toLower() == expectedAnswer.simplified().toLower())
       (*logStream) << "=> Pass\n";
    else
       (*logStream) << "=> Fail\n";
  }
}
*/

bool
AimlParser::loadSubs(const QString &filename)
{
  QDomDocument doc;
  QFile file(filename);
  bool succeed = file.open(QFile::ReadOnly);
  Q_ASSERT(succeed);
  if (!succeed)
    return false;
  if (!doc.setContent(&file)) {
    file.close();
    qDebug() << "Error while parsing " << filename;
    Q_ASSERT(0);
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList subsList = docElem.elementsByTagName("substitution");
  for (int i = 0; i < subsList.count(); i++) {
    QDomElement n = subsList.item(i).toElement();
    subOld.append(QRegExp(n.namedItem("old").firstChild().nodeValue(), Qt::CaseInsensitive));
    subNew.append(n.namedItem("new").firstChild().nodeValue());
  }
  return true;
}

bool
AimlParser::loadVars(const QString &filename, bool bot)
{
  QDomDocument doc;
  QFile file( filename );
  bool succeed = file.open(QFile::ReadOnly);
  if (!succeed) {
    qDebug() << "Failed to open aiml file:" << filename;
    Q_ASSERT(0);
    return false;
  }
  if (!doc.setContent(&file)) {
    file.close();
    qDebug() << "Error while parsing:" << filename;
    Q_ASSERT(0);
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList setList = docElem.elementsByTagName ("set");
  for (int i = 0; i < setList.count(); i++) {
    QDomElement n = setList.item(i).toElement();
    QString name = n.attribute("name");
    QString value = n.firstChild().nodeValue();
    if (bot)
      botVarValue[name] = value;
    else
      parameterValue[name] = value;
  }
  return true;
}

bool
AimlParser::saveVars(const QString &filename)
{
  QDomDocument doc;
  QDomElement root = doc.createElement("vars");
  doc.appendChild(root);

  for (QMap<QString, QString>::ConstIterator
       it = parameterValue.begin(); it != parameterValue.end(); ++it) {
    QDomElement setElem = doc.createElement("set");
    setElem.setAttribute("name", it.key());
    QDomText text = doc.createTextNode(it.value());
    setElem.appendChild(text);
    root.appendChild(setElem);
  }
  // Backup the file first
  QFile fileBackup( filename + ".bak" );
  if (!fileBackup.open(QFile::WriteOnly)) {
    Q_ASSERT(0);
    return false;
  }
  QTextStream tsBackup(&fileBackup);
  QFile file(filename);
  if (!file.open(QFile::ReadOnly)) {
    Q_ASSERT(0);
    return false;
  }

  tsBackup << QString(file.readAll());
  fileBackup.close();
  file.close();

  // Now, save it!
  if (!file.open(QFile::WriteOnly)) {
    Q_ASSERT(0);
    return false;
  }
  QTextStream ts(&file);
  ts << doc.toString();
  file.close();
  return true;
}

bool
AimlParser::loadAiml(const QString &filename)
{
  QDomDocument doc("mydocument");
  QFile file(filename);
  if (!file.open(QFile::ReadOnly)) {
    Q_ASSERT(0);
    return false;
  }

  QXmlInputSource src(&file);
  QXmlSimpleReader reader;
  reader.setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData", true);

  QString msg;
  int line, col;
  if (!doc.setContent(&src, &reader, &msg, &line, &col)) {
    file.close();
    qDebug() << "Error while parsing " << filename << ":" <<msg << "(line " << line << " - col " << col << ")\n";
    Q_ASSERT(0);
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList categoryList = docElem.elementsByTagName ("category");
  for (int i = 0; i < categoryList.count(); i++) {
    QDomNode n = categoryList.item(i);
    parseCategory(&n);
  }
  return true;
}

// Parses a category and creates a correspondant element
void
AimlParser::parseCategory(QDomNode *categoryNode)
{
  Q_ASSERT(categoryNode);
  QDomNode patternNode = categoryNode->namedItem("pattern");
  QString pattern = resolveNode(&patternNode);
  pattern = ::normalized(pattern);
  //find where to insert the new node
  QStringList words = pattern.split(' ');
  Node *whereToInsert = &root_;
  for (QStringList::ConstIterator it = words.begin(); it != words.end(); ++it) {
    bool found = false;
    foreach (Node *child, whereToInsert->children) {
      if (child->word == *it) {
        whereToInsert = child;
        found = true;
        break;
      }
    }
    if (!found) {
      for (; it != words.end(); ++it ) {
        Node *n = new Node;
        n->word = *it;
        n->parent = whereToInsert;
        int index = 0;
        if (*it == "*")
          index = whereToInsert->children.count();
        else if (*it != "_"
                 && whereToInsert->children.count()
                 && whereToInsert->children.at(0)->word == "_")
          index = 1;
        whereToInsert->children.insert(index, n);
        whereToInsert = n;
      }
      break;
    }
  }

  //Now insert the leaf
  Leaf *leaf = new Leaf;
  leaf->parent = whereToInsert;
  QDomNode thatNode = categoryNode->namedItem("that");
  if (!thatNode.isNull()) {
    leaf->that = thatNode.firstChild().toText().nodeValue();
    leaf->that = ::normalized(leaf->that);
  }
  leaf->tpl = categoryNode->namedItem("template");
  QDomNode parentNode = categoryNode->parentNode();
  if (!parentNode.isNull() && parentNode.nodeName() == "topic") {
    leaf->topic = parentNode.toElement().attribute("name");
    leaf->topic = ::normalized(leaf->topic);
  }
  int index = 0;
  int leafWeight = !leaf->that.isEmpty() + !leaf->topic.isEmpty() * 2;
  foreach (Leaf *childLeaf, whereToInsert->leaves) {
    int childLeafWeight = !childLeaf->that.isEmpty() + !childLeaf->topic.isEmpty() * 2;
    if (leafWeight >= childLeafWeight)
       break;
     index++;
  }
  whereToInsert->leaves.insert(index, leaf);
}

//recursively replace all the values & return the QString result
QString
AimlParser::resolveNode(QDomNode* node, const QStringList &capturedTexts,
  const QStringList &capturedThatTexts, const QStringList &capturedTopicTexts)
{
  QString result("");
  QString nodeName = node->nodeName();
  QDomElement element = node->toElement();
  if (nodeName == "random") {
    QList<QDomNode> childNodes = ::elementsByTagName(node, "li");
    int childCount = childNodes.count();
    int random = rand() % childCount;
    QDomNode child = childNodes[random];
    result = resolveNode(&child, capturedTexts, capturedThatTexts, capturedTopicTexts);
  } else if (nodeName == "condition") {
    QString name("");
    int condType = 2;
    if (element.hasAttribute("name")) {
      condType = 1;
      name = element.attribute("name");
      if (element.hasAttribute("value")) {
        condType = 0;
        QString value = element.attribute("value").toUpper();
        QStringList dummy;
        if (exactMatch(value, parameterValue[name].toUpper(), dummy)) {
          //dirty trick to avoid infinite loop !
          element.setTagName("parsedCondition");
          result = resolveNode(&element, capturedTexts, capturedThatTexts, capturedTopicTexts);
          element.setTagName("condition");
        }
      }
    } if (condType) {
      QList<QDomNode> childNodes = ::elementsByTagName(node, "li");
      for (int i = 0; i < childNodes.count(); i++) {
        QDomNode n = childNodes[i];
        if (n.toElement().hasAttribute("value")) {
          if (condType == 2)
            name = n.toElement().attribute("name");
          QString value = n.toElement().attribute("value").toUpper();
          QStringList dummy;
          if (exactMatch(value, parameterValue[name].toUpper(), dummy)) {
            result = resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
            break;
          }
        } else {
          result = resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
          break;
        }
      }
    }
  } else {
    QDomNode n = node->firstChild();
    while (!n.isNull()) {
      result += resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
      n = n.nextSibling();
    }
    if (node->isText())
      result = node->toText().nodeValue();
    else if (nodeName == "set")
        parameterValue[element.attribute("name")] = result.trimmed();
    else if (nodeName == "srai")
      result = getResponse(result, true);
    else if (nodeName == "think")
      result = "";
    else if (nodeName == "system")
      result = executeCommand(result);
    else if (nodeName == "learn") {
      loadAiml(result);
      result = "";
    } else if (nodeName == "uppercase")
      result = result.toUpper();
    else if (nodeName == "lowercase")
      result = result.toLower();
    else if (!node->hasChildNodes()) {
      if (nodeName == "star") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedTexts.count() ? capturedTexts[index] : QString("");
      } else if (nodeName == "thatstar") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedThatTexts.count() ? capturedThatTexts[index] : QString("");
      } else if (nodeName == "topicstar") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedTopicTexts.count() ? capturedTopicTexts[index] : QString("");
      } else if (nodeName == "that") {
        QString indexStr = element.attribute("index", "1,1");
        if (!indexStr.contains(","))
           indexStr = "1," + indexStr;
        int index1 = indexStr.section(',', 0, 0).toInt()-1;
        int index2 = indexStr.section(',', 1, 1).toInt()-1;
        result = (index1 < thatList.count()) && (index2 < thatList[index1].count()) ?
           thatList[index1][index2] : QString("");
      } else if (nodeName == "sr")
        result = getResponse(capturedTexts.count() ? capturedTexts[0] : QString(""), true);
      else if (nodeName == "br" || nodeName == "html:br")
        result = "\n";
      else if ( nodeName == "get" )
        result = parameterValue[element.attribute("name")];
      else if (nodeName == "bot")
        result = botVarValue[element.attribute("name")];
      else if (nodeName == "person" || nodeName == "person2" || nodeName == "gender" )
        result = capturedTexts.count() ? capturedTexts[0] : QString("");
      else if (nodeName == "input") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < inputList.count() ? inputList[index] : QString("");
      }
      //the following just to avoid warnings !
      else if (nodeName == "li")
        ;
      else
        qDebug() << "Warning: unknown tag \"" + nodeName + "\"\n";
    }
    else if (nodeName == "template" || nodeName == "pattern" || nodeName == "li"
         || nodeName == "person" || nodeName == "person2" || nodeName == "gender"
         || nodeName == "parsedCondition")
      //the following just to avoid warnings !
      ;
    else
      qDebug() << "Warning: unknown tag \"" + nodeName + "\"\n";
  }
  return result;
}

QString
AimlParser::getResponse(const QString &text, bool srai)
{
#ifdef AIML_DEBUG
  qDebug() << (srai ? "::SRAI: " : "::User Input: ") << text;
#endif // AIML_DEBUG

  QString input = text;

  // perform substitutions for input string
  QList<QRegExp>::Iterator itOld = subOld.begin();
  QStringList::Iterator itNew = subNew.begin();
  for (; itOld != subOld.end(); ++itOld, ++itNew )
    input.replace(*itOld, *itNew);
  if (!srai)
  {
    inputList.prepend(input);
    if (inputList.count() > AIML_MAX_LIST_LENGTH)
      inputList.pop_back();
  }

  QStringList capturedTexts, capturedThatTexts, capturedTopicTexts;
  QString curTopic = parameterValue["topic"];
  curTopic = ::normalized(curTopic);
  Leaf *leaf = NULL;
  QString result("");
  QStringList sentences = input.split(QRegExp("[\\.\\?!;]"));
  QStringList::Iterator sentence = sentences.begin();
  forever {
    //normalizeString(*sentence);
    *sentence = (*sentence).toLower();
    QStringList inputWords = sentence->split(' ');
    QStringList::ConstIterator it = inputWords.begin();
    if (!root_.match(it, inputWords, thatList.count() && thatList[0].count() ?
      thatList[0][0] : QString(""), curTopic, capturedThatTexts, capturedTopicTexts, leaf))
      return "Internal Error!";
    Node *parentNode = leaf->parent;
    QString matchedPattern = parentNode->word;
    while (parentNode->parent->parent) {
      parentNode = parentNode->parent;
      matchedPattern = parentNode->word + " " + matchedPattern;
    }
#ifdef AIML_DEBUG
    qDebug() << "::Matched pattern: [" << matchedPattern + "]";
    if (!leaf->that.isEmpty())
      qDebug() << " - Matched that: [" << leaf->that << "]";
    if (!leaf->topic.isEmpty())
      qDebug() << " - Matched topic: [" << leaf->topic << "]";
#endif // AIML_DEBUG

    capturedTexts.clear();
    exactMatch(matchedPattern, *sentence, capturedTexts);
    //strip whitespaces from the beggining and the end of result
    if (visitedNodeList.contains(&leaf->tpl))
        result += "ProgramQ: Infinite loop detected!";
    else {
      visitedNodeList.append(&leaf->tpl);
      result += resolveNode(&leaf->tpl, capturedTexts, capturedThatTexts, capturedTopicTexts).trimmed();
    }
    sentence++;
    if (sentence != sentences.end())
      result += " ";
    else
      break;
  }
  if (!srai) {
    QString tempResult = result.simplified();
    //get the sentences of the result splitted by: . ? ! ; and "arabic ?"
    QStringList thatSentencesList = tempResult.split(QRegExp("[\\.\\?!;]"));
    QStringList inversedList;
    for (QStringList::Iterator it = thatSentencesList.begin(); it != thatSentencesList.end(); ++it)
    {
        //perform substitutions for that string
        itOld = subOld.begin();
        itNew = subNew.begin();
        for (; itOld != subOld.end(); ++itOld, ++itNew )
          tempResult.replace(*itOld, *itNew);
        *it = ::normalized(*it);
      inversedList.prepend(*it);
    }
    thatList.prepend(inversedList);
    if (thatList.count() > AIML_MAX_LIST_LENGTH)
      thatList.pop_back();
    visitedNodeList.clear();
  }

#ifdef AIML_DEBUG
  qDebug()<< "::Result: " << result;
#endif // AIML_DEBUG

  return result;
}

QString AimlParser::executeCommand(const QString &commandStr)
{
  QString returnString("");
#ifdef AIML_DEBUG
  qDebug() << "Executing " << commandStr;
#endif // AIML_DEBUG

#ifdef Q_WS_WIN
  STARTUPINFO si;
  SECURITY_ATTRIBUTES sa;
  SECURITY_DESCRIPTOR sd;
  PROCESS_INFORMATION pi;
  HANDLE read_pipe, write_pipe;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  int fd, create;
  OSVERSIONINFO osv;
  osv.dwOSVersionInfoSize = sizeof(osv);

  ::GetVersionEx(&osv);

  if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
    sa.lpSecurityDescriptor = &sd;
  }
  else /* Pipe will use ACLs from default descriptor */
    sa.lpSecurityDescriptor = NULL;

  /* Create a new pipe with system's default buffer size */
  if (!CreatePipe(&read_pipe, &write_pipe, &sa, 0)) {
    Q_ASSERT(0);
    return "";
  }

  GetStartupInfo(&si);

  /* Sets the standard output handle for the process to the
  handle specified in hStdOutput */
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

  si.hStdOutput = write_pipe;
  si.hStdError  = (HANDLE) _get_osfhandle (2);
  si.wShowWindow = 0;
  QString exec = QString("cmd.exe /c \"") + commandStr + "\"";
  create = CreateProcess(NULL,                   // The full path of app to launch
               (unsigned short*)const_cast<char*>(exec.toAscii().constData()),  // Command line parameters
               NULL,                   // Default process security attributes
               NULL,                   // Default thread security attributes
               TRUE,                   // Inherit handles from the parent
               0,                    // Normal priority
               NULL,                   // Use the same environment as the parent
               NULL,                   // Use app's directory as current
               &si,                  // Startup Information
               &pi);                   // Process information stored upon return

  if (!create) {
    Q_ASSERT(0);
    return "";
  }

  /* Associates a file descriptor with the stdout pipe */
  fd = _open_osfhandle(/*(intptr_t)*/(long int)read_pipe, _O_RDONLY);

  /* Close the handle that we're not going to use */
  CloseHandle(write_pipe);

  if (!fd) {
    Q_ASSERT(0);
    return "";
  }

  /* Open the pipe stream using its file descriptor */
  FILE *file = _fdopen(fd, "r");

  if(!file) {
    Q_ASSERT(0);
    return "";
  }
#else

  FILE *file = popen(commandStr.toAscii(), "r");
  if (!file) {
    Q_ASSERT(0);
    return "";
  }
#endif

  char c = 0;
  while (c != EOF)
  {
    c = (char)getc(file);

    if (isascii(c))
      returnString += c;
  }

  fclose(file);

#ifdef AIML_DEBUG
  qDebug() << "Execution succeeded with result: " << returnString;
#endif // AIML_DEBUG

  return returnString;
}

// EOF

// - Helpers -
/*

namespace { // anonymous

  //A much faster replacement of regExp.exactMatch(str)
  //it also captures the words corresponding to the wildcards * & _
  bool
  exactMatch(QString regExp, QString str, QStringList &capturedText)
  {
    QStringList regExpWords = regExp.split(' ');
    QStringList strWords = str.split(' ');
    if ((!regExpWords.count() || !strWords.count()) && (regExpWords.count() != strWords.count()))
      return false;
    if (regExpWords.count() > strWords.count())
      return false;
    QStringList::ConstIterator regExpIt = regExpWords.begin();
    QStringList::ConstIterator strIt = strWords.begin();
    while ((strIt != strWords.end()) && (regExpIt != regExpWords.end())) {
      if ( (*regExpIt == "*") || (*regExpIt == "_") ) {
        regExpIt++;
        QStringList capturedStr;
        if (regExpIt != regExpWords.end()) {
          QString nextWord = *regExpIt;
          if ( (nextWord != "*") && (nextWord != "_") ) {
            while (true) {
              if (*strIt == nextWord)
                break;
              capturedStr += *strIt;
              if (++strIt == strWords.end())
                return false;
            }
          } else {
            capturedStr += *strIt;
            regExpIt -;
          }
        } else {
          forever {
            capturedStr += *strIt;
            if (++strIt == strWords.end())
              break;
          }
          capturedText += capturedStr.join(" ");
          return true;
        }
        capturedText += capturedStr.join(" ");
      }
      else if (*regExpIt != *strIt)
        return false;
      regExpIt++;
      strIt++;
    }
    return (regExpIt == regExpWords.end()) && (strIt == strWords.end());
  }

} // anonymous namespace

namespace { // anonymous, tree
  QList<QDomNode>
  elementsByTagName(QDomNode *node, const QString& tagName)
  {
    QList<QDomNode> list;
    QDomNodeList childNodes = node->toElement().elementsByTagName(tagName);
    for (int i = 0; i < childNodes.count(); i++)
    {
      QDomNode n = childNodes.item(i);
      if (n.parentNode() == *node)
          list.append(n);
    }
    return list;
  }

} // anonymous namespace

// - Nodes and leaves -

bool
AimlParser::Node::match(QStringList::const_iterator input, const QStringList &inputWords,
  const QString &currentThat, const QString &currentTopic, QStringList &capturedThatTexts,
  QStringList &capturedTopicTexts, Leaf *&leaf)
{
  if (input == inputWords.end())
     return false;

  if ((word == "*") || (word == "_")) {
      ++input;
      for (;input != inputWords.end(); ++input) {
          foreach (Node *child, children) {
            if (child->match(input, inputWords, currentThat, currentTopic, capturedThatTexts,
              capturedTopicTexts, leaf))
              return true;
          }
      }
  } else {
      if (!word.isEmpty()) {
           if (word != *input)
          return false;
         ++input;
      }
      foreach (Node *child, children) {
        if (child->match(input, inputWords, currentThat, currentTopic, capturedThatTexts,
          capturedTopicTexts, leaf))
          return true;
      }
  }
  if (input == inputWords.end()) {
    foreach (leaf, leaves) {
      capturedThatTexts.clear();
      capturedTopicTexts.clear();
      if ( (!leaf->that.isEmpty() && !exactMatch(leaf->that, currentThat, capturedThatTexts)) ||
          (!leaf->topic.isEmpty() && !exactMatch(leaf->topic, currentTopic, capturedTopicTexts)) )
        continue;
      return true;
    }
  }

  return false;
}

void
AimlParser::Node::debug(QTextStream* logStream, int indent)
{
  QString indentStr = QString().fill('\t', indent);
  (*logStream) << indentStr << word << " :\n";
  foreach (Node* child, children)
    child->debug(logStream, indent + 1);
  indentStr = QString().fill('\t', indent + 1);
  foreach (Leaf* leaf, leaves)
    (*logStream) << indentStr << "<topic-" << leaf->topic << " that-" << leaf->that << ">\n";
}

// - Parser -

void
AimlParser::runRegression()
{
  QDomDocument doc;
  QFile file( "utils/TestSuite.xml" );
  if ( !file.open(QFile::ReadOnly) )
    return;
  if ( !doc.setContent( &file ) )
  {
    file.close();
    (*logStream) << "Error while parsing " << file.fileName() << "\n";
    return;
  }
  file.close();

  (*logStream) << "Regression running:\n";

  loadAiml("utils/TestSuite.aiml");

  QDomElement docElem = doc.documentElement();
  QDomNodeList testCaseList = docElem.elementsByTagName ("TestCase");
  for (int i = 0; i < testCaseList.count(); i++)
  {
    QDomElement n = testCaseList.item(i).toElement();
    QString description = n.namedItem("Description").firstChild().nodeValue();
    QString input = n.namedItem("Input").firstChild().nodeValue();

    QString expectedAnswer = "";
    QDomNode child = n.namedItem("ExpectedAnswer").firstChild();
    while (!child.isNull()) {
      if (child.isText())
         expectedAnswer += child.toText().nodeValue();
      child = child.nextSibling();
    }
    (*logStream) << "===========================================================================\n";
    (*logStream) << "::Description: " + description + "\n";
    (*logStream) << "::Expected answer: " + expectedAnswer + "\n";
    QString answer = getResponse(input);
    if (answer.simplified().toLower() == expectedAnswer.simplified().toLower())
       (*logStream) << "=> Pass\n";
    else
       (*logStream) << "=> Fail\n";
  }
}

void
AimlParser::displayTree()
{
  root.debug(logStream);
}

void
AimlParser::normalizeString(QString &str)
{
  QString newStr;
  for (int i = 0; i < str.length(); i++) {
    QChar c = str.at(i);
    if (c.isLetterOrNumber() || (c == '*') || (c == '_') || (c == ' '))
      newStr += c.toLower();
  }
  str = newStr;
}

AimlParser::AimlParser(QTextStream *logStream)
  : logStream(logStream)
{
  indent = 0;
  root.parent = 0;
  QTime currentTime = QTime::currentTime();
  int val = currentTime.msec() + currentTime.second() + currentTime.minute();
  srand(val);
}

AimlParser::~AimlParser() {}

bool
AimlParser::loadSubstitutions(const QString &filename)
{
  QDomDocument doc;
  QFile file( filename );
  if ( !file.open(QFile::ReadOnly) )
    return false;
  if ( !doc.setContent( &file ) ) {
    file.close();
    (*logStream) << "Error while parsing " << filename << "\n";
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList subsList = docElem.elementsByTagName ("substitution");
  for (int i = 0; i < subsList.count(); i++) {
    QDomElement n = subsList.item(i).toElement();
    subOld.append(QRegExp(n.namedItem("old").firstChild().nodeValue(), Qt::CaseInsensitive));
    subNew.append(n.namedItem("new").firstChild().nodeValue());
  }
  return true;
}

bool
AimlParser::loadVars(const QString &filename, bool bot)
{
  QDomDocument doc;
  QFile file( filename );
  if ( !file.open(QFile::ReadOnly) )
    return false;
  if ( !doc.setContent( &file ) )
  {
    file.close();
    (*logStream) << "Error while parsing " << filename << "\n";
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList setList = docElem.elementsByTagName ("set");
  for (int i = 0; i < setList.count(); i++)
  {
    QDomElement n = setList.item(i).toElement();
    QString name = n.attribute("name");
    QString value = n.firstChild().nodeValue();
    if (bot)
      botVarValue[name] = value;
    else
      parameterValue[name] = value;
  }
  return true;
}

bool
AimlParser::saveVars(const QString &filename)
{
  QDomDocument doc;
  QDomElement root = doc.createElement("vars");
  doc.appendChild(root);

  QMap<QString, QString>::ConstIterator it;
  for ( it = parameterValue.begin(); it != parameterValue.end(); ++it )
  {
    QDomElement setElem = doc.createElement("set");
    setElem.setAttribute("name", it.key());
    QDomText text = doc.createTextNode(it.value());
    setElem.appendChild(text);
    root.appendChild(setElem);
  }
  //Backup the file first
  QFile fileBackup( filename + ".bak" );
  if ( !fileBackup.open(QFile::WriteOnly) )
    return false;
  QTextStream tsBackup(&fileBackup);
  QFile file( filename );
  if ( !file.open(QFile::ReadOnly) )
    return false;
  tsBackup << QString(file.readAll());
  fileBackup.close();
  file.close();
  //now, save it!
  if ( !file.open(QFile::WriteOnly) )
    return false;
  QTextStream ts(&file);
  ts << doc.toString();
  file.close();
  return true;
}

bool
AimlParser::loadAiml(const QString &filename)
{
  QDomDocument doc( "mydocument" );
  QFile file( filename );
  if ( !file.open(QFile::ReadOnly) )
    return false;

  QXmlInputSource src(&file);
  QXmlSimpleReader reader;
  reader.setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData", true);

  QString msg;
  int line, col;
  if ( !doc.setContent( &src, &reader, &msg, &line, &col ) ) {
    file.close();
    (*logStream) << "Error while parsing " << filename << ":" <<msg << "(line " << line << " - col " << col << ")\n";
    return false;
  }
  file.close();

  QDomElement docElem = doc.documentElement();
  QDomNodeList categoryList = docElem.elementsByTagName ("category");
  for (int i = 0; i < categoryList.count(); i++) {
    QDomNode n = categoryList.item(i);
    parseCategory(&n);
  }
  return true;
}

// parses a category and creates a correspondant element
void
AimlParser::parseCategory(QDomNode* categoryNode)
{
  QDomNode patternNode = categoryNode->namedItem("pattern");
  QString pattern = resolveNode(&patternNode);
  normalizeString(pattern);
  //find where to insert the new node
  QStringList words = pattern.split(' ');
  Node *whereToInsert = &root;
  for ( QStringList::ConstIterator it = words.begin(); it != words.end(); ++it ) {
    bool found = false;
    foreach (Node* child, whereToInsert->children) {
      if (child->word == *it) {
        whereToInsert = child;
        found = true;
        break;
      }
    }
    if (!found) {
      for (; it != words.end(); ++it ) {
        Node *n = new Node;
        n->word = *it;
        n->parent = whereToInsert;
            int index = 0;
        if (*it == "*")
           index = whereToInsert->children.count();
        else if ((*it != "_") && whereToInsert->children.count() &&
           (whereToInsert->children.at(0)->word == "_"))
               index = 1;
            whereToInsert->children.insert(index, n);
        whereToInsert = n;
      }
      break;
    }
  }

  //Now insert the leaf
  Leaf *leaf = new Leaf;
  leaf->parent = whereToInsert;
  QDomNode thatNode = categoryNode->namedItem("that");
  if (!thatNode.isNull()) {
    leaf->that = thatNode.firstChild().toText().nodeValue();
    normalizeString(leaf->that);
  }
  leaf->tpl = categoryNode->namedItem("template");
  QDomNode parentNode = categoryNode->parentNode();
  if (!parentNode.isNull() && (parentNode.nodeName() == "topic")) {
    leaf->topic = parentNode.toElement().attribute("name");
    normalizeString(leaf->topic);
  }
  int index = 0;
  int leafWeight = !leaf->that.isEmpty() + !leaf->topic.isEmpty() * 2;
  foreach (Leaf* childLeaf, whereToInsert->leaves) {
    int childLeafWeight = !childLeaf->that.isEmpty() + !childLeaf->topic.isEmpty() * 2;
    if (leafWeight >= childLeafWeight)
       break;
     index++;
  }
  whereToInsert->leaves.insert(index, leaf);
}

//recursively replace all the values & return the QString result
QString
AimlParser::resolveNode(QDomNode* node, const QStringList &capturedTexts,
  const QStringList &capturedThatTexts, const QStringList &capturedTopicTexts)
{
  QString result("");
  QString nodeName = node->nodeName();
  QDomElement element = node->toElement();
  if (nodeName == "random") {
    QList<QDomNode> childNodes = elementsByTagName(node, "li");
    int childCount = childNodes.count();
    int random = rand() % childCount;
    QDomNode child = childNodes[random];
    result = resolveNode(&child, capturedTexts, capturedThatTexts, capturedTopicTexts);
  } else if (nodeName == "condition") {
    QString name("");
    int condType = 2;
    if (element.hasAttribute("name")) {
      condType = 1;
      name = element.attribute("name");
      if (element.hasAttribute("value")) {
        condType = 0;
        QString value = element.attribute("value").toUpper();
        QStringList dummy;
        if (exactMatch(value, parameterValue[name].toUpper(), dummy)) {
          //dirty trick to avoid infinite loop !
          element.setTagName("parsedCondition");
          result = resolveNode(&element, capturedTexts, capturedThatTexts, capturedTopicTexts);
          element.setTagName("condition");
        }
      }
    } if (condType) {
      QList<QDomNode> childNodes = elementsByTagName(node, "li");
      for (int i = 0; i < childNodes.count(); i++) {
        QDomNode n = childNodes[i];
        if (n.toElement().hasAttribute("value")) {
          if (condType == 2)
            name = n.toElement().attribute("name");
          QString value = n.toElement().attribute("value").toUpper();
          QStringList dummy;
          if (exactMatch(value, parameterValue[name].toUpper(), dummy)) {
            result = resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
            break;
          }
        } else {
          result = resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
          break;
        }
      }
    }
  } else {
    QDomNode n = node->firstChild();
    while (!n.isNull()) {
      result += resolveNode(&n, capturedTexts, capturedThatTexts, capturedTopicTexts);
      n = n.nextSibling();
    }
    if (node->isText())
      result = node->toText().nodeValue();
    else if (nodeName == "set")
        parameterValue[element.attribute("name")] = result.trimmed();
    else if (nodeName == "srai")
      result = getResponse(result, true);
    else if (nodeName == "think")
      result = "";
    else if (nodeName == "system")
      result = executeCommand(result);
    else if (nodeName == "learn") {
      loadAiml(result);
      result = "";
    } else if (nodeName == "uppercase")
      result = result.toUpper();
    else if (nodeName == "lowercase")
      result = result.toLower();
    else if (!node->hasChildNodes()) {
      if (nodeName == "star") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedTexts.count() ? capturedTexts[index] : QString("");
      } else if (nodeName == "thatstar") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedThatTexts.count() ? capturedThatTexts[index] : QString("");
      } else if (nodeName == "topicstar") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < capturedTopicTexts.count() ? capturedTopicTexts[index] : QString("");
      } else if (nodeName == "that") {
        QString indexStr = element.attribute("index", "1,1");
        if (!indexStr.contains(","))
           indexStr = "1," + indexStr;
        int index1 = indexStr.section(',', 0, 0).toInt()-1;
        int index2 = indexStr.section(',', 1, 1).toInt()-1;
        result = (index1 < thatList.count()) && (index2 < thatList[index1].count()) ?
           thatList[index1][index2] : QString("");
      } else if (nodeName == "sr")
        result = getResponse(capturedTexts.count() ? capturedTexts[0] : QString(""), true);
      else if ( (nodeName == "br") || (nodeName == "html:br") )
        result = "\n";
      else if ( nodeName == "get" )
        result = parameterValue[element.attribute("name")];
      else if ( nodeName == "bot")
        result = botVarValue[element.attribute("name")];
      else if ( (nodeName == "person") || (nodeName == "person2") || (nodeName == "gender") )
        result = capturedTexts.count() ? capturedTexts[0] : QString("");
      else if (nodeName == "input") {
        int index = element.attribute("index", "1").toUInt() - 1;
        result = index < inputList.count() ? inputList[index] : QString("");
      }
      //the following just to avoid warnings !
      else if (nodeName == "li")
        ;
      else
        (*logStream) << "Warning: unknown tag \"" + nodeName + "\"\n";
    } else if ((nodeName == "template") || (nodeName == "pattern") || (nodeName == "li")
         || (nodeName == "person") || (nodeName == "person2") || (nodeName == "gender")
         || (nodeName == "parsedCondition"))
      //the following just to avoid warnings !
      ;
    else
      (*logStream) << "Warning: unknown tag \"" + nodeName + "\"\n";
  }
  return result;
}

QString
AimlParser::getResponse(const QString &_todo, bool srai)
{
  QString input = _todo;
  // debug
  if (srai)
    indent ++;

  QString indentSpace = QString().fill(' ', 2*indent);
  (*logStream) << (!srai ? "\n" : "") + indentSpace + (srai ? "::SRAI: " : "::User Input: ") +
    input + "\n";
  //perform substitutions for input string
  QList<QRegExp>::Iterator itOld = subOld.begin();
  QStringList::Iterator itNew = subNew.begin();
  for (; itOld != subOld.end(); ++itOld, ++itNew )
    input.replace(*itOld, *itNew);
  if (!srai)
  {
    inputList.prepend(input);
    if (inputList.count() > MAX_LIST_LENGTH)
      inputList.pop_back();
  }

  QStringList capturedTexts, capturedThatTexts, capturedTopicTexts;
  QString curTopic = parameterValue["topic"];
  normalizeString(curTopic);
  Leaf *leaf = NULL;
  QString result("");
  QStringList sentences = input.split(QRegExp("[\\.\\?!;\\x061f]"));
  QStringList::Iterator sentence = sentences.begin();
  forever {
    //normalizeString(*sentence);
    *sentence = (*sentence).toLower();
    QStringList inputWords = sentence->split(' ');
    QStringList::ConstIterator it = inputWords.begin();
    if (!root.match(it, inputWords, thatList.count() && thatList[0].count() ?
      thatList[0][0] : QString(""), curTopic, capturedThatTexts, capturedTopicTexts, leaf))
      return "Internal Error!";
    Node *parentNode = leaf->parent;
    QString matchedPattern = parentNode->word;
    while (parentNode->parent->parent) {
      parentNode = parentNode->parent;
      matchedPattern = parentNode->word + " " + matchedPattern;
    }
    (*logStream) << indentSpace + "::Matched pattern: [" + matchedPattern + "]";
    if (!leaf->that.isEmpty())
       (*logStream) << " - Matched that: [" + leaf->that + "]";
    if (!leaf->topic.isEmpty())
       (*logStream) << " - Matched topic: [" + leaf->topic + "]";
    (*logStream) << "\n";
    capturedTexts.clear();
    exactMatch(matchedPattern, *sentence, capturedTexts);
    //strip whitespaces from the beggining and the end of result
    if (visitedNodeList.contains(&leaf->tpl))
        result += "ProgramQ: Infinite loop detected!";
    else {
      visitedNodeList.append(&leaf->tpl);
      result += resolveNode(&leaf->tpl, capturedTexts, capturedThatTexts, capturedTopicTexts).trimmed();
    }
    sentence++;
    if (sentence != sentences.end())
      result += " ";
    else
      break;
  }
  if (!srai) {
    QString tempResult = result.simplified();
    //get the sentences of the result splitted by: . ? ! ; and "arabic ?"
    QStringList thatSentencesList = tempResult.split(QRegExp("[\\.\\?!;\\x061f]"));
    QStringList inversedList;
    for (QStringList::Iterator it = thatSentencesList.begin(); it != thatSentencesList.end(); ++it)
    {
        //perform substitutions for that string
        itOld = subOld.begin();
        itNew = subNew.begin();
        for (; itOld != subOld.end(); ++itOld, ++itNew )
          tempResult.replace(*itOld, *itNew);
        normalizeString(*it);
      inversedList.prepend(*it);
    }
    thatList.prepend(inversedList);
    if (thatList.count() > MAX_LIST_LENGTH)
      thatList.pop_back();
    visitedNodeList.clear();
  }
  //debug
  (*logStream) << indentSpace + "::Result: " + result + "\n";
  if (srai)
    indent -;

  return result;
}

QString AimlParser::executeCommand(const QString &commandStr)
{
  QString returnString("");
  QString spaceIndent = QString().fill(' ', 2*indent);
  (*logStream) << spaceIndent + "Executing \"" + commandStr + "\" ...\n";

#ifdef Q_WS_WIN

  STARTUPINFO si;
  SECURITY_ATTRIBUTES sa;
  SECURITY_DESCRIPTOR sd;
  PROCESS_INFORMATION pi;
  HANDLE read_pipe, write_pipe;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;
  int fd, create;
  OSVERSIONINFO osv;
  osv.dwOSVersionInfoSize = sizeof(osv);

  GetVersionEx(&osv);

  if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
    sa.lpSecurityDescriptor = &sd;
  }
  else // Pipe will use ACLs from default descriptor
    sa.lpSecurityDescriptor = NULL;

  // Create a new pipe with system's default buffer size
  if (!CreatePipe(&read_pipe, &write_pipe, &sa, 0)) {
    (*logStream) << spaceIndent + "Execution failed !\n";
    return "";
  }

  GetStartupInfo(&si);

  // Sets the standard output handle for the process to the handle specified in hStdOutput
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

  si.hStdOutput = write_pipe;
  si.hStdError  = (HANDLE) _get_osfhandle (2);
  si.wShowWindow = 0;
  QString exec = QString("cmd.exe /c \"") + commandStr + "\"";
  create = CreateProcess(NULL,                   // The full path of app to launch
               (unsigned short*)const_cast<char*>(exec.toAscii().constData()),  // Command line parameters
               NULL,                   // Default process security attributes
               NULL,                   // Default thread security attributes
               TRUE,                   // Inherit handles from the parent
               0,                    // Normal priority
               NULL,                   // Use the same environment as the parent
               NULL,                   // Use app's directory as current
               &si,                  // Startup Information
               &pi);                   // Process information stored upon return

  if (!create) {
    (*logStream) << spaceIndent + "Execution failed !\n";
    return "";
  }

  // Associates a file descriptor with the stdout pipe
  fd = _open_osfhandle((long int)read_pipe, _O_RDONLY);//(intptr_t)

  // Close the handle that we're not going to use
  CloseHandle(write_pipe);

  if (!fd)
  {
    (*logStream) << spaceIndent + "Execution failed !\n";
    return "";
  }

  // Open the pipe stream using its file descriptor
  FILE *file = _fdopen(fd, "r");

  if(!file)
  {
    (*logStream) << spaceIndent + "Execution failed !\n";
    return "";
  }
#else

  FILE *file = popen(commandStr, "r");
  if (!file) {
    (*logStream) << spaceIndent + "Execution failed !\n";
    return "";
  }
#endif

  char c = 0;
  while (c != EOF)
  {
    c = (char)getc(file);

    if (isascii(c))
      returnString += c;
  }

  fclose(file);

  (*logStream) << spaceIndent + "Execution succeeded with result: \"" + returnString + "\"\n";

  return returnString;
}
*/
