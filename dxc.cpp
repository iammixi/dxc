#include "dxcC.h"

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
	try
	{
		CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
		CORBA::Object_var tmp = orb->string_to_object("file:///var/run/dxcd.ior");
		dxc::client_var client = dxc::client::_narrow(tmp.in ());

		if (CORBA::is_nil (client.in ()))
		{
			ACE_ERROR_RETURN ((	LM_DEBUG,
						"Nil dxc::client reference\n"),
						1);
		}

		client->post_job(L"gcc do some!", 123, 456);
		ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned %C\n",
					""));
		orb->destroy ();
	}
	catch (const CORBA::Exception& ex)
	{
		ex._tao_print_exception ("Exception caught:");
		return 1;
	}

	return 0;
}

