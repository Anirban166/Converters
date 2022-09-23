# R equivalent for what I wrote in Python:
library(stringr)

pattern  <-  "\\/[a-z]{2,4}\\.[a-z]*(\\.?-?[a-z0-9]*){1,}"
# Alternate pattern: "/[a-z]{2,3}\\.[a-z0-9-]+\\.[a-z0-9-]+"
# Example targets:
# /net.byte-sift-1.12.0
# /org.antlr-antlr4-runtime-4.9.3
# /io.github.classgraph-classgraph-4.8.130
# /org.xmlunit-xmlunit-matchers-2.8.3
# /org.jsoup-jsoup-1.14.3
commitData     <- read.csv("Anirban166GitHubCommits.csv")
commitMessages <- commitData$messages
sum(str_detect(commitMessages, pattern))
pos <- grep(pattern, commitMessages)
commitMessages <- str_replace(commitMessages, pattern, "42 Commits ahead, 69 commits behind")
commitMessages[pos]