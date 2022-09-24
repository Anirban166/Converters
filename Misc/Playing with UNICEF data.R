# Ani, ac4743@nau.edu, Fall '22
library(dplyr)
library(knitr)
library(readr)
library(tidyr)
library(readxl)
library(naniar)
library(ggplot2)
library(stringr)
library(ggridges)

# I first extract the data which is in an excel file (.xlsx format) provided to us. In doing so, I skip the first six lines since they do not contain observations (not properly organized too or following a pattern like the rest rows). I then write this data (which is in the form of a list) to a csv file.
raw.mortality.list <- read_excel("unicef_allindicators.xlsx", skip = 6)
write_csv(raw.mortality.list, path = "unicef_allindicators.csv")
raw.mortality <- read.csv("unicef_allindicators.csv")

# The data is in the wide format since it has extra columns which should be observations instead of variables, thus:
fixed.format.raw.mortality <- raw.mortality %>% tidyr::pivot_longer(cols = !c(1:3), names_to = "categoryYear", values_to = "mortalityEstimates")
fixed.format.raw.mortality <- fixed.format.raw.mortality %>% mutate(type = str_extract(categoryYear, '[:alnum:]{3,4}'), year = str_extract(categoryYear, '[0-9]{4}'))
# To fix this specific untidy nature of the data, I'm using `pivot_longer()` to reshape it by making it have more rows as appropriate. First three columns apart, the rest include values for observations based on two separate variables (category and year) which are combined in the column header as a conjunction of their strings separated by a dot. Thus, I separately create these two distinct variables by using regular expressions to extract the category (an alphanumeric string of 3 to 4 characters in length) and the year (a 4-digit number) from the combined string.

sub.mortality <- fixed.format.raw.mortality %>% filter(Uncertainty.bounds. != "Lower" & Uncertainty.bounds. != "Upper") %>% select(-Uncertainty.bounds.)
mortality <- sub.mortality %>% mutate(country = as.factor(CountryName), type = as.factor(type), year = as.integer(year), value = as.numeric(mortalityEstimates)) %>% select(country, type, value, year)
levels(mortality$type) <- c("U5MR", "IMR", "NMR", "U5deaths", "Ideaths", "Ndeaths")

# Q: Which year range have more missing data? (you can define what "range" means based on your exploration) Why do you think the pattern you see is (or it is not) reasonable?
# A: First, I collected the number of missing values across all countries and across all types per year in a data frame. I then plot the gathered data which shows a trend of more missing data in the earlier years, and next to none after the 1990s. I would say that the year range 1951-1960 has more amount of missing data. 
explore.mortality <- mortality %>% group_by(year) %>% summarise(Number.of.NA.values = sum(is.na(value)))
ggplot(explore.mortality, aes(Number.of.NA.values, year)) + geom_point()
explore.mortality %>% filter(year >= 1951 & year <= 1960) 
# All values for the year range above are much greater than the mean (291.8485) too!

# I say this based on the plot and for good reasons after a bit of research - To begin with, UNICEF has been collecting records for 76 years till date, since 1946. Given that the early days of the initiative were still in the phases of forging a plan for data collection, it first makes sense for most of the missing data to be in those early years of less planning and outreach. The countries that fall under the list of UNICEF kept gradually expanding, and thus when the newer ones were incorporated, the data for the previous years where they were not in the list are marked as NAs.
# Moreover, I select that particular year range for historial reasons mentioned online too such as in [this webpage](https://www.nobelprize.org/prizes/peace/1965/unicef/history/).
# Here's the line from there that supports this - "UNICEF continued to meet emergency needs, but at the same time moved into the long-range benefit approach during the period 1951-1960." 
# In summary, what this means is that they changed their goal from providing relief resources and funds for the children affected by the war (World War II) to long-term goals that benefit all children but come with wider goals. This meant operations on a global scale to support children in various ways. Coming back to what I mentioned in the first paragraph, this meant that eventually the number of countries that UNICEF would provide support to kept increasing over the years, but the data from past years for new countries added to the list would be missing. This fortifies my aforementioned rationale, but there's also the fact that some countries even when supported could not have accurate mortality estimate records due to the lack of reliable registration data. The United Nations Interagency Group for Child Mortality Estimation (UN IGME) was formed to tackle this issue through better monitoring primarily, but this group only came into existence after 2004.

# Filtering the data from the 90s (as they're complete), and using a `naniar` based grammar of graphics construct which shows the number of missing values per variable in a plot: (no missing data in this dataframe!)
mortality90 <- mortality %>% filter(year >= 1990)
gg_miss_var(mortality90)

# To answer my question "Is mortality rate or the number of deaths of the neonatal type a better estimate in fortifying the statement "Neonatal Mortality estimates for the European countries Switzerland, Netherlands, and France were high in value between the years 1990 and 2000 (inclusive of both)?":
neonatal.df <- mortality %>% filter(type %in% c("Ndeaths", "NMR") & country %in% c("Switzerland", "Netherlands", "France") & year >= 1990 & year <= 2000)
ggplot(neonatal.df , aes(x = value, y = type, fill = country)) + xlim(0, 7000) + theme_ridges() + theme(legend.position = "right") + geom_density_ridges(alpha = 0.5) + labs(x = "Mortality estimate value", y = "Estimate type")