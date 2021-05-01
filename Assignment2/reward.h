int gift()
{
	int x = rand() % 100;
	if (x < 60) return 1; // arrow
	else if (x < 80) return 2; // med kit
	else if (x < 90) return 3;  // bomb!!
	else return 4;   // score booster
}