import re
import pandas as pd

pattern = "\\/[a-z]{2,4}\\.[a-z]*(\\.?-?[a-z0-9]*){1,}"
csvfile = pd.read_csv('Anirban166GitHubCommits.csv')
commitMessages = csvfile.messages

matchCount = 0
for i in range(len(commitMessages)):
  matchCount += len(re.findall(pattern, commitMessages[i]))  
print("Total number of matches:", matchCount)

print("\nRegEx-satisfying commit messages with the capture positions in matched rows:")
finalList = []
positions = []
for i in range(len(commitMessages)):
  if bool(re.search(pattern, commitMessages[i])):
    finalList.append(commitMessages[i])
    positions.append(re.search(pattern, commitMessages[i]).span())
for i in range(len(finalList)):
  print("\nMatch number", i + 1, "is:")    
  print(finalList[i])
  print("The position (start, end) where the RegEx was captured in the row is:", positions[i])

print("\nMatched comments after replacement of RegEx-captured strings for the final part:")
for i in range(len(finalList)):
  print(re.sub(pattern, "42 Commits ahead, 69 commits behind", finalList[i]))  