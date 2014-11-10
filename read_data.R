library(ggplot2)

# Given photodiode readings and triggers, keep those readings for which the state was ON
assign_photodiode <- function(pd, triggers) {
  on_times <- triggers[triggers[, 'Reading'] == "ON", 'Time']
  off_times <- triggers[triggers[, 'Reading'] == "OFF", 'Time']
  valid_times <- do.call(c, list(mapply(seq, on_times, off_times)))
  pd[pd[, 'Time'] %in% valid_times, ]
}

# Given photodiode readings and triggers, keep those readings for which the state was OFF
assign_photodiode_off <- function(pd, red_triggers, ir_triggers) {
  red_off <- red_triggers[red_triggers[, 'Reading'] == "OFF", 'Time']
  ir_on <- ir_triggers[ir_triggers[, 'Reading'] == "ON", 'Time']
  ir_off <- ir_triggers[ir_triggers[, 'Reading'] == "OFF", 'Time']
  red_on <- red_triggers[red_triggers[, 'Reading'] == "ON", 'Time']

  red_dark <- do.call(c, list(mapply(seq, red_off, ir_on)))
  ir_dark <- do.call(c, list(mapply(seq, ir_off[-length(ir_off)], red_on[-1])))
  dark <- union(red_dark, ir_dark)
  pd[pd[, 'Time'] %in% dark, ]
}

# Plot photodiode readings when red or IR LED is on
plot_pd <- function(red, ir, dark=NULL) {
  if(is.null(dark)) print(ggplot(red, aes(Time, Reading)) + geom_line(aes(color="Red")) + geom_line(data=ir, aes(color="IR")) + labs(color="LED"))
  else print(ggplot(red, aes(Time, Reading)) + geom_line(aes(color="Red")) + geom_line(data=ir, aes(color="IR"))+ geom_line(data=dark, aes(color="Off")) + labs(color="LED"))
}


# Read original data
pd <- read.csv("photodiode.dat", sep="\t")
red_triggers <- read.csv("red.dat", sep="\t")
ir_triggers <- read.csv("ir.dat", sep="\t")

# Split photodiode data based on LED state
red <- assign_photodiode(pd, red_triggers)
ir <- assign_photodiode(pd, ir_triggers)
dark <- assign_photodiode_off(pd, red_triggers, ir_triggers)

# Select photodiode readings for which the system was covered
plot_pd(red, ir)
start_time <- as.numeric(readline("Dark start time: "))
end_time <- as.numeric(readline("Dark end time: "))
red_valid <- red[red[, 'Time'] > start_time & red[, 'Time'] < end_time, ]
ir_valid <- ir[ir[, 'Time'] > start_time & ir[, 'Time'] < end_time, ]
dark_valid <- dark[dark[, 'Time'] > start_time & dark[, 'Time'] < end_time, ]
plot_pd(red_valid, ir_valid)
