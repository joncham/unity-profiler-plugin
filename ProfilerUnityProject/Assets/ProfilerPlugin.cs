using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class ProfilerPlugin : MonoBehaviour
{

	[DllImport("ProfilerPlugin")]
	static extern int Initialize();

	void Start ()
	{
		Initialize();
	}
	

	void Update ()
	{
		
	}
}
