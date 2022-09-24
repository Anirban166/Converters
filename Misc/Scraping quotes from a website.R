# Ani, ac4743@nau.edu, Fall '22
library(httr)
library(rvest)
library(stringr)
library(tidyverse)

# Creating an empty dataframe for storing the extracted quote-data:
quotes.df <- data.frame(matrix(nrow = 0, ncol = 1))
colnames(quotes.df) = c("quotes")
# Scraping the first 10 pages: (I made a function to do the extraction job)
extractQuotes <- function(page)
{ 
  return(page %>% html_nodes(".quote") %>% html_text()) # html_text2() doesn't work to separate.
}
for (x in 1:10) 
{
  url <- paste("https://quotes.toscrape.com/page/", x, sep = '')
  Sys.sleep(0.5) # Delay to avoid sending too many requests fast enough to cause a connection problem.
  # closeAllConnections() # Just to avoid possible connection timeouts that I faced earlier.
  page <- read_html(url)
  quotes <- extractQuotes(page)
  # Binding the scraped quotes to our dataframe:
  quotes.df <- rbind(quotes.df, as.data.frame(quotes)) # quotes.df would be a list of 10 (text/quotes)
}
# quotes.df at present is a list of 10 (text/quotes), so I'm breaking that single column into three separate ones - the quotes, author names and list of tags, in order:
quotes <- str_split(quotes.df$quotes, '(?<=\\.”)\\s+by\\s+', simplify = TRUE) # Using a positive lookbehind for the anything before the end quote within a capture group
authorTags <- quotes[,2]
authors <- str_split(authorTags, '\\(about\\)\\s+(Tags:?)\\s+', simplify = TRUE)
tags <- authors[,2]
quotes <- quotes[,1]
authors <- authors[,1]
quotes.df <- cbind.data.frame(quotes, authors, tags)

# Reshaping the dataframe to separate the tags into several columns:
tags <- str_split(quotes.df$tags, '\\s', simplify = TRUE)
quotes.df <- cbind.data.frame(quotes.df, tags)
quotes.df <- quotes.df %>% pivot_longer(cols = !c(1:3), values_to = "tag", values_drop_na = TRUE)
quotes.df <- quotes.df %>% filter(tag != "") %>% select(-tags, -name)

# Creating a visualization for the frequency of tags that are mentioned more than twice:
filterTag <- quotes.df %>% group_by(tag) %>% filter(n() > 2) %>% select(tag)
ggplot(filterTag, aes(x = tag)) + geom_bar() +
theme(axis.text.x = element_text(angle = 75, hjust = 1))