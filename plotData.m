% Read all of the data in the TDMS file.
data = tdmsread("Logs/Test4.tdms");

%% Encoder

% `data` contains one cell per group. Each cell contains a table with one
% column per channel. In this case, the first channel in the first group
% contains the time, and the second channel contains the position etc.
time1 = table2array(data{1,1}(:,1));
position = table2array(data{1,1}(:,2));
velocity = table2array(data{1,1}(:,3));

figure;
yyaxis left;
% 611 is the conversion ratio from counts on the encoder to degrees.
plot(time1-time1(1),position/611);
ylim([-20,20]);
ylabel("Position (degrees)");
hold on;
yyaxis right;
plot(time1-time1(1),velocity/611*1000);
ylim([-50,50]);
ylabel("Velocity (degrees/s)");
xlabel("Time (seconds)");
xlim([7,11.5]);
title("Steering motor readings");
hold off;

%% Gyro
time2 = table2array(data{1,2}(:,1));
rollRate = table2array(data{1,2}(:,2));

figure;
plot(time2-time2(1),rollRate);
ylabel("Roll rate");
xlabel("Time (seconds)");
title("Gyro");

%% Steering motor

time3 = table2array(data{1,3}(:,1));
pwm = table2array(data{1,3}(:,2));

figure;
plot(time3-time3(1),pwm);
ylim([0,1]);
ylabel("Duty cycle");
xlabel("Time (seconds)");
title("Steering motor control");

%% Combined

figure;
yyaxis left;
plot(time3-time2(1),pwm);
ylim([0,1]);
ylabel("Duty cycle");
hold on;
yyaxis right;
plot(time2-time2(1),rollRate);
ylabel("Roll rate");
ylim([-10,10]);
xlim([7,11.5]);
xlabel("Time (seconds)");
title("Duty cycle and Roll rate");










