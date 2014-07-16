#include <vector>

#include "dxcS.h"

class job : public virtual POA_dxc::job
{
public:
	job(const CORBA::WChar* command, dxc::pid caller, dxc::pid make)
		: command_(command), caller_(caller), make_(make)
	{	
	}

	virtual ::CORBA::WChar * command_line()
	{
		return ACE_OS::strdup(command_);
	}

	virtual ::dxc::pid caller()
	{
		return caller_;
	}

	virtual ::dxc::pid make()
	{
		return make_;
	}

	virtual ::CORBA::Boolean spawned()
	{
		return 1;
	}

	virtual ::CORBA::Boolean terminated()
	{
		return 1;
	}

	virtual ::CORBA::Long returncode()
	{
		return 0;
	}

	virtual ::CORBA::Boolean read_out(::CORBA::WString_out msg)
	{
		return 0;
	}

        virtual ::CORBA::Boolean read_err(::CORBA::WString_out msg)
        {
                return 0;
        }
private:
	const CORBA::WChar* 	command_;
	dxc::pid	caller_;
	dxc::pid	make_;
};

class client : public virtual POA_dxc::client
{
public:
        virtual dxc::job_ptr post_job(  const CORBA::WChar*   command_line,
                                        dxc::pid              caller,
                                        dxc::pid              make)
        {
                ACE_DEBUG ((LM_DEBUG, L"received post_job: %s\n",
                                       command_line));

                job* job_impl = new job(command_line, caller, make);
                jobs.push_back(job_impl);

                return dxc::job_ptr(job_impl);
        }
private:
        std::vector<job*> jobs;
};

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
	try
	{
		CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
		CORBA::Object_var poa_object = orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in ());
		if (CORBA::is_nil (root_poa.in ()))
		{
			ACE_ERROR_RETURN ((	LM_ERROR, 
						" (%P|%t) Panic: nil RootPOA\n"),
						1);
		}
		PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

		client *client_impl = 0;
		ACE_NEW_RETURN (client_impl,
				client(),
				1);
		PortableServer::ServantBase_var owner_transfer(client_impl);
		PortableServer::ObjectId_var id = root_poa->activate_object (client_impl);
		CORBA::Object_var object = root_poa->id_to_reference(id.in ());
		dxc::client_var client = dxc::client::_narrow(dxc::client::_narrow(object.in()));
		CORBA::String_var ior = orb->object_to_string (client.in());
		
		// Output the IOR to the ior_output_file
		FILE *output_file= ACE_OS::fopen ("/var/run/dxcd.ior", "w");
		if (output_file == 0)
		{
			ACE_ERROR_RETURN (	(LM_ERROR,
						"Cannot open output file for writing IOR: %s\n",
						"/var/run/dxcd.ior"),
						1);
		}
		ACE_OS::fprintf (output_file, "%s", ior.in ());
		ACE_OS::fclose (output_file);		
		
		poa_manager->activate ();
		orb->run ();

	}
	catch (const CORBA::Exception& ex)
	{
		ex._tao_print_exception ("Exception caught:");
		return 1;
	}
	return 0;
}
