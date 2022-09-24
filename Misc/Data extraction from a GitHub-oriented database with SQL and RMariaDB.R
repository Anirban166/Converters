# Ani, ac4743@nau.edu, Fall '22
library(RMariaDB)
library(tidyverse)

# Creating the connection: (variables for the user and password are in my local env., change as desired)
GitHub.database <- dbConnect(RMariaDB::MariaDB(), user = raven, password = mendokusai, dbname = 'GitHub.database', host = 'localhost')
GitHub.database %>% dbListTables()
prs <- GitHub.database %>% dbReadTable("prs")
repos <- GitHub.database %>% dbReadTable("repos")

# Creating a dataframe from an anonymous table with joined content of repositories and their pull request stats:
sqlQuery <- "select * from prs natural join repos;"
cursor <- GitHub.database %>% dbSendQuery(sqlQuery)
pullRequests <- dbFetch(cursor)
dbClearResult(cursor)
head(pullRequests)

# Discarding the standalone prs table and saving the combined data instead in the database:
GitHub.database %>% dbRemoveTable("prs")
dbWriteTable(GitHub.database, "pullRequestData", pullRequests)
GitHub.database %>% dbListFields("pullRequestData")

# Adding a row to repos, and subsequently overwriting the table in the database before closing the connection:
repos[nrow(repos) + 1, ] <- c("Anirban166", "Test", 69, "2022-12-05")
dbWriteTable(GitHub.database, "repos", repos, overwrite = TRUE)
GitHub.database %>% dbReadTable("repos")
dbDisconnect(GitHub.database)