# Patterns for replacements:
headingsPattern <- "(^[#]+)(.+)"
hyperlinksPattern <- "\\[(\\w+)\\]\\((.+)\\)"
boldPattern <- "\\*\\*(.+)\\*\\*"
italicPattern <- "\\*(.+)\\*"
inlineCodePattern <- "\\`{1}([^\\`].+)\\`{1}"
strikethroughPattern <- "\\~{2}(.+)\\~{2}"
indentedCodePattern <- "(^[\\s]+)([\\w]+.+)"
orderedListPattern <- "(\\d\\.\\s(.+))"
codeBlockStartingPattern <- "\\`{3}\\{r\\s?.+\\}"
codeBlockEndingPattern <- "\\`{3}"

# Headings:
matches <- do.call(rbind, str_match_all(html.file$X, headingsPattern))
# First one's outta loop just to initialize html.file$Y:
html.file$Y <- str_replace(html.file$X, matches[1,1],
paste('<h', as.character(nchar(matches[1,2])), '>', matches[1,3], '</h', as.character(nchar(matches[1,2])), '>', sep = ""))
for(x in 2:length(matches[,2])) # Skipping the first one since it's done above.
{
  replacement <- paste('<h', as.character(nchar(matches[x,2])), '>', matches[x,3], '</h', as.character(nchar(matches[x,2])), '>', sep = "")
  html.file$Y <- str_replace(html.file$Y, matches[x,1], replacement)
}

# Hyperlinks:
matches <- do.call(rbind, str_match_all(html.file$Y, hyperlinksPattern))
for(x in 1:length(matches[,2])) {
  replacement <- paste("<a href=\"", matches[x,3],"\">",matches[x,2],"</a>", sep="")
  html.file$Y <- str_replace_all(html.file$Y,  coll(paste(matches[x,1])), replacement)
}

# Bold highlights:
matches <- do.call(rbind, str_match_all(html.file$Y, boldPattern))
for(x in 1:length(matches[,2]))
{
  replacement <- paste('<strong>', matches[x,2], '</strong>', sep = "")
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}

# Italic highlights:
matches <- do.call(rbind, str_match_all(html.file$Y, italicPattern))
for(x in 1:length(matches[,2]))
{
  replacement <- paste('<em>', matches[x,2], '</em>', sep = "")
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}

# In-line code:
matches <- do.call(rbind, str_match_all(html.file$Y, inlineCodePattern))
for(x in 1:length(matches[,2]))
{
  replacement <- paste('<code>', matches[x,2], '</code>', sep = "")
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}

# Strikethrough fonts:
matches <- do.call(rbind, str_match_all(html.file$Y, strikethroughPattern))
for(x in 1:length(matches[,2]))
{
  replacement <- paste('<del>', matches[x,2], '</del>', sep = "")
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}

# Indented code:
matches <- do.call(rbind, str_match_all(html.file$Y, indentedCodePattern))
for(x in 1:length(matches[,2]))
{
  replacement <- paste('<pre><code>', matches[x,3], '</code></pre>', sep = "")
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}

# Ordered lists:
matches <- do.call(rbind, str_match_all(html.file$Y, orderedListPattern))
for(x in 1:length(matches[,2]))
{ 
  # First match: (needs an <ol> before <li> or first item's start)
  if(x == 1) 
  {
    replacement <- paste('<ol><li>', matches[x,3], '</li>', sep = "")
    html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
  }  
  # Last match: (needs an </ol> after </li> or last item's end)
  if(x == length(matches[,2])) 
  {
    replacement <- paste('<li>', matches[x,3], '</li></ol>', sep = "")
    html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
  }  
  replacement <- paste('<li>', matches[x,3], '</li>', sep = "")
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}

# For unordered bullet lists, I devised a novel approach worthy of a nobel prize. But I suggest the reader to write his/her own code for this one (=
# I'll help you with the RegEx patterns:
# For the outermost level (basically when the line starts with a bullet), you can use "\\+\\s(.+)".
# For inner levels (n spaces in before the bullet for the row), you can use "\\s{n}(\\+|\\-|\\*)\\s(.+)".

# Code blocks:
# Replacing the starting part: (i.e., ```{r ...})
matches <- do.call(rbind, str_match_all(html.file$Y, codeBlockStartingPattern))
for(x in 1:length(matches[,1]))
{
  replacement <- paste('<code>')
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}
# Replacing the ending part: (i.e., just triple backticks)
matches <- do.call(rbind, str_match_all(html.file$Y, codeBlockEndingPattern))
for(x in 1:length(matches[,1]))
{
  replacement <- paste('</code>')
  html.file$Y <- str_replace(html.file$Y, coll(matches[x,1]), replacement)
}