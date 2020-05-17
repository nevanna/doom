#include "engine.h"

int main()
{
	//loop back
	// 127.0.0.1
	printf("hfov %f, vfov %f\n", hfov, vfov);
	t_engine e;
	ft_menu(&e);
	ft_general_loop(&e);
	ft_close(&e);

	return (0);
}
