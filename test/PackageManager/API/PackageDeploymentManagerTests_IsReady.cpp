// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "PackageDeploymentManagerTests.h"

#include <IsWindowsVersion.h>

namespace TD = ::Test::Diagnostics;
namespace TB = ::Test::Bootstrap;
namespace TP = ::Test::Packages;
namespace TPF = ::Test::Packages::Framework;
namespace TPM = ::Test::Packages::Main;
namespace TPMT = ::Test::PackageManager::Tests;

namespace Test::PackageManager::Tests
{
    class PackageDeploymentManagerTests_IsReady : PackageDeploymentManagerTests_Base
    {
    public:
        BEGIN_TEST_CLASS(PackageDeploymentManagerTests_IsReady)
            TEST_CLASS_PROPERTY(L"ThreadingModel", L"MTA")
            TEST_CLASS_PROPERTY(L"IsolationLevel", L"Class")    /****SEEME****/
            TEST_CLASS_PROPERTY(L"RunAs", L"RestrictedUser")
        END_TEST_CLASS()

        TEST_CLASS_SETUP(ClassSetup)
        {
            ::TD::DumpExecutionContext();
            if (!::WindowsVersion::IsWindows10_20H1OrGreater())
            {
                WEX::Logging::Log::Result(WEX::Logging::TestResults::Skipped, L"PackageDeploymentManager requires >= 20H1 (Vibranium). Skipping tests");
                return true;
            }

            TD::DumpExecutionContext();

            RemovePackage_Blue();
WEX::Logging::Log::Comment(L"S2");
            RemovePackage_Green();
WEX::Logging::Log::Comment(L"S3");
            RemovePackage_Redder();
WEX::Logging::Log::Comment(L"S4");
            RemovePackage_Red();
WEX::Logging::Log::Comment(L"S5");
            ::TB::Setup();
WEX::Logging::Log::Comment(L"S6");
            return true;
        }

        TEST_CLASS_CLEANUP(ClassCleanup)
        {
            RemovePackage_Blue();
            RemovePackage_Green();
            RemovePackage_Redder();
            RemovePackage_Red();
            ::TB::Cleanup();
            return true;
        }

        TEST_METHOD(IsPackageReady_InvalidParameter)
        {
            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            try
            {
                PCWSTR packageFullName{ L"Not a valid Package Full Name" };
                packageDeploymentManager.IsPackageReady(packageFullName);
                VERIFY_FAIL(L"Success is not expected");
            }
            catch (winrt::hresult_error& e)
            {
                VERIFY_ARE_EQUAL(E_INVALIDARG, e.code(), WEX::Common::String().Format(L"0x%X %s", e.code(), e.message().c_str()));
            }
        }

        TEST_METHOD(IsPackageReady_PackageFullName_NoSuchPackage_No)
        {
            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            PCWSTR c_packageFullName{ L"Does.Not.Exist_1.2.3.4_neutral__1234567890abc" };

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageReady(c_packageFullName));
        }

        TEST_METHOD(IsPackageReady_PackageFullName_NotInstalled_No)
        {
            RemovePackage_Red();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            PCWSTR packageFullName{ ::TPF::Red::GetPackageFullName() };

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageReady(packageFullName));
        }

        TEST_METHOD(IsPackageReady_PackageFullName_Registered_Yes)
        {
            AddPackage_Red();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            PCWSTR packageFullName{ ::TPF::Red::GetPackageFullName() };

            VERIFY_IS_TRUE(packageDeploymentManager.IsPackageReady(packageFullName));
        }

        TEST_METHOD(IsPackageReady_PackageFullName_OlderRegistered_No)
        {
            AddPackage_Red();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            PCWSTR packageFullName{ ::TPF::Redder::GetPackageFullName() };

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageReady(packageFullName));
        }

        TEST_METHOD(IsPackageReady_PackageFullName_NewerRegistered_Yes)
        {
            AddPackage_Redder();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            PCWSTR packageFullName{ ::TPF::Red::GetPackageFullName() };

            VERIFY_IS_TRUE(packageDeploymentManager.IsPackageReady(packageFullName));

            RemovePackage_Redder();
        }

        TEST_METHOD(IsPackageReady_PackageFamilyName_InvalidParameter)
        {
            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            try
            {
                PCWSTR c_packageFamilyName{ L"Does.Not.Exist_1234567890abc" };
                packageDeploymentManager.IsPackageReady(c_packageFamilyName);
                VERIFY_FAIL(L"Success is not expected");
            }
            catch (winrt::hresult_error& e)
            {
                VERIFY_ARE_EQUAL(E_INVALIDARG, e.code(), WEX::Common::String().Format(L"0x%X %s", e.code(), e.message().c_str()));
            }
        }

        TEST_METHOD(IsPackageSetReady_InvalidParameter)
        {
WEX::Logging::Log::Comment(L"P1");
            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            try
            {
WEX::Logging::Log::Comment(L"P2");
                winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
WEX::Logging::Log::Comment(L"P3");
                packageDeploymentManager.IsPackageSetReady(packageSet);
WEX::Logging::Log::Comment(L"P4");
                VERIFY_FAIL(L"Success is not expected");
WEX::Logging::Log::Comment(L"P5");
            }
            catch (winrt::hresult_error& e)
            {
WEX::Logging::Log::Comment(L"P6");
                VERIFY_ARE_EQUAL(E_INVALIDARG, e.code(), WEX::Common::String().Format(L"0x%X %s", e.code(), e.message().c_str()));
            }

WEX::Logging::Log::Comment(L"P10");
            try
            {
WEX::Logging::Log::Comment(L"P11");
                winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
WEX::Logging::Log::Comment(L"P12");
                PCWSTR c_packageSetId{ L"Does.Not.Exist" };
WEX::Logging::Log::Comment(L"P13");
                packageSet.Id(c_packageSetId);
WEX::Logging::Log::Comment(L"P14");

                packageDeploymentManager.IsPackageSetReady(packageSet);
WEX::Logging::Log::Comment(L"P15");
                VERIFY_FAIL(L"Success is not expected");
WEX::Logging::Log::Comment(L"P16");
            }
            catch (winrt::hresult_error& e)
            {
WEX::Logging::Log::Comment(L"P17");
                VERIFY_ARE_EQUAL(E_INVALIDARG, e.code(), WEX::Common::String().Format(L"0x%X %s", e.code(), e.message().c_str()));
WEX::Logging::Log::Comment(L"P18");
            }

WEX::Logging::Log::Comment(L"20");
            try
            {
WEX::Logging::Log::Comment(L"21");
                winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
WEX::Logging::Log::Comment(L"22");
                PCWSTR c_packageSetId{ L"Does.Not.Exist" };
                packageSet.Id(c_packageSetId);
WEX::Logging::Log::Comment(L"23");
                winrt::Microsoft::Windows::Management::Deployment::PackageSetItem packageSetItem;
                packageSet.Items().Append(packageSetItem);

WEX::Logging::Log::Comment(L"24");
                packageDeploymentManager.IsPackageSetReady(packageSet);
WEX::Logging::Log::Comment(L"25");
                VERIFY_FAIL(L"Success is not expected");
WEX::Logging::Log::Comment(L"26");
            }
            catch (winrt::hresult_error& e)
            {
WEX::Logging::Log::Comment(L"27");
                VERIFY_ARE_EQUAL(E_INVALIDARG, e.code(), WEX::Common::String().Format(L"0x%X %s", e.code(), e.message().c_str()));
WEX::Logging::Log::Comment(L"28");
            }

WEX::Logging::Log::Comment(L"30");
            try
            {
WEX::Logging::Log::Comment(L"31");
                winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
WEX::Logging::Log::Comment(L"32");
                PCWSTR c_packageSetId{ L"Does.Not.Exist" };
                packageSet.Id(c_packageSetId);
WEX::Logging::Log::Comment(L"33");
                winrt::Microsoft::Windows::Management::Deployment::PackageSetItem packageSetItem;
WEX::Logging::Log::Comment(L"34");
                PCWSTR c_packageFamilyName{ L"Not a valid Package Family Name" };
                packageSetItem.PackageFamilyName(c_packageFamilyName);
WEX::Logging::Log::Comment(L"35");
                packageSet.Items().Append(packageSetItem);

WEX::Logging::Log::Comment(L"36");
                packageDeploymentManager.IsPackageSetReady(packageSet);
WEX::Logging::Log::Comment(L"37");
                VERIFY_FAIL(L"Success is not expected");
WEX::Logging::Log::Comment(L"38");
            }
            catch (winrt::hresult_error& e)
            {
WEX::Logging::Log::Comment(L"39");
                VERIFY_ARE_EQUAL(E_INVALIDARG, e.code(), WEX::Common::String().Format(L"0x%X %s", e.code(), e.message().c_str()));
            }

WEX::Logging::Log::Comment(L"40");
            try
            {
WEX::Logging::Log::Comment(L"41");
                winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
WEX::Logging::Log::Comment(L"42");
                PCWSTR c_packageSetId{ L"Does.Not.Exist" };
                packageSet.Id(c_packageSetId);
WEX::Logging::Log::Comment(L"43");
                winrt::Microsoft::Windows::Management::Deployment::PackageSetItem packageSetItem;
WEX::Logging::Log::Comment(L"44");
                PCWSTR c_packageUriAsString{ L"https://doesnotexist.com/assemble.msix" };
                winrt::Windows::Foundation::Uri packageUri{ c_packageUriAsString };
WEX::Logging::Log::Comment(L"45");
                packageSetItem.PackageUri(packageUri);
WEX::Logging::Log::Comment(L"46");
                packageSet.Items().Append(packageSetItem);

WEX::Logging::Log::Comment(L"47");
                packageDeploymentManager.IsPackageSetReady(packageSet);
WEX::Logging::Log::Comment(L"48");
                VERIFY_FAIL(L"Success is not expected");
WEX::Logging::Log::Comment(L"49");
            }
            catch (winrt::hresult_error& e)
            {
WEX::Logging::Log::Comment(L"4A");
                VERIFY_ARE_EQUAL(E_INVALIDARG, e.code(), WEX::Common::String().Format(L"0x%X %s", e.code(), e.message().c_str()));
WEX::Logging::Log::Comment(L"4B");
            }
        }

        TEST_METHOD(IsPackageSetReady_1_NoSuchPackage_No)
        {
            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"Does.Not.Exist" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem packageSetItem;
            PCWSTR c_packageFamilyName{ L"Does.Not.Exist_1234567890abc" };
            packageSetItem.PackageFamilyName(c_packageFamilyName);
            PCWSTR c_packageUriAsString{ L"file://c:/does/not/exist.msix" };
            winrt::Windows::Foundation::Uri packageUri{ c_packageUriAsString };
            packageSetItem.PackageUri(packageUri);
            packageSet.Items().Append(packageSetItem);

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }

        TEST_METHOD(IsPackageSetReady_1_NotInstalled_No)
        {
            RemovePackage_Red();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }

        TEST_METHOD(IsPackageSetReady_1_Registered_Yes)
        {
            AddPackage_Red();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);

            VERIFY_IS_TRUE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }

        TEST_METHOD(IsPackageSetReady_1_OlderRegistered_No)
        {
            AddPackage_Red();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem redder{ Make_PackageSetItem(::TPF::Redder::GetPackageFullName(), ::TPF::Redder::c_packageDirName) };
            packageSet.Items().Append(redder);

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }

        TEST_METHOD(IsPackageSetReady_1_NewerRegistered_Yes)
        {
            AddPackage_Redder();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);

            VERIFY_IS_TRUE(packageDeploymentManager.IsPackageSetReady(packageSet));

            RemovePackage_Redder();
        }

        TEST_METHOD(IsPackageSetReady_N_NotInstalled_No)
        {
            RemovePackage_Red();
            RemovePackage_Green();
            RemovePackage_Blue();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem green{ Make_PackageSetItem(::TPF::Green::GetPackageFullName(), ::TPF::Green::c_packageDirName) };
            packageSet.Items().Append(green);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem blue{ Make_PackageSetItem(::TPF::Blue::GetPackageFullName(), ::TPF::Blue::c_packageDirName) };
            packageSet.Items().Append(blue);

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }

        TEST_METHOD(IsPackageSetReady_N_Registered_Yes)
        {
            AddPackage_Red();
            AddPackage_Green();
            AddPackage_Blue();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem green{ Make_PackageSetItem(::TPF::Green::GetPackageFullName(), ::TPF::Green::c_packageDirName) };
            packageSet.Items().Append(green);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem blue{ Make_PackageSetItem(::TPF::Blue::GetPackageFullName(), ::TPF::Blue::c_packageDirName) };
            packageSet.Items().Append(blue);

            VERIFY_IS_TRUE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }

        TEST_METHOD(IsPackageSetReady_N_OlderRegistered_No)
        {
            AddPackage_Red();
            AddPackage_Green();
            AddPackage_Blue();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem redder{ Make_PackageSetItem(::TPF::Redder::GetPackageFullName(), ::TPF::Redder::c_packageDirName) };
            packageSet.Items().Append(redder);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem green{ Make_PackageSetItem(::TPF::Green::GetPackageFullName(), ::TPF::Green::c_packageDirName) };
            packageSet.Items().Append(green);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem blue{ Make_PackageSetItem(::TPF::Blue::GetPackageFullName(), ::TPF::Blue::c_packageDirName) };
            packageSet.Items().Append(blue);

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }

        TEST_METHOD(IsPackageSetReady_N_NewerRegistered_Yes)
        {
            AddPackage_Redder();
            AddPackage_Green();
            AddPackage_Blue();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem green{ Make_PackageSetItem(::TPF::Green::GetPackageFullName(), ::TPF::Green::c_packageDirName) };
            packageSet.Items().Append(green);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem blue{ Make_PackageSetItem(::TPF::Blue::GetPackageFullName(), ::TPF::Blue::c_packageDirName) };
            packageSet.Items().Append(blue);

            VERIFY_IS_TRUE(packageDeploymentManager.IsPackageSetReady(packageSet));

            RemovePackage_Redder();
        }

        TEST_METHOD(IsPackageSetReady_N_RegisteredAndNotInstalled_No)
        {
            AddPackage_Red();
            RemovePackage_Green();

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem green{ Make_PackageSetItem(::TPF::Green::GetPackageFullName(), ::TPF::Green::c_packageDirName) };
            packageSet.Items().Append(green);

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageSetReady(packageSet));
        }
    };

    class PackageDeploymentManagerTests_IsReady_Elevated : PackageDeploymentManagerTests_Base
    {
    public:
        BEGIN_TEST_CLASS(PackageDeploymentManagerTests_IsReady_Elevated)
            TEST_CLASS_PROPERTY(L"ThreadingModel", L"MTA")
            TEST_CLASS_PROPERTY(L"RunAs", L"ElevatedUser")
        END_TEST_CLASS()

        TEST_CLASS_SETUP(ClassSetup)
        {
            ::TD::DumpExecutionContext();
            if (!::WindowsVersion::IsWindows10_20H1OrGreater())
            {
                WEX::Logging::Log::Result(WEX::Logging::TestResults::Skipped, L"PackageDeploymentManager requires >= 20H1 (Vibranium). Skipping tests");
                return true;
            }
WEX::Logging::Log::Comment(L"S1");
            RemovePackage_Blue();
WEX::Logging::Log::Comment(L"S2");
            RemovePackage_Green();
WEX::Logging::Log::Comment(L"S3");
            RemovePackage_Redder();
WEX::Logging::Log::Comment(L"S4");
            RemovePackage_Red();
WEX::Logging::Log::Comment(L"S5");
            ::TB::Setup();
WEX::Logging::Log::Comment(L"S6");
            return true;
        }

        TEST_CLASS_CLEANUP(ClassCleanup)
        {
            RemovePackage_Blue();
            RemovePackage_Green();
            RemovePackage_Redder();
            RemovePackage_Red();
            ::TB::Cleanup();
            return true;
        }

        TEST_METHOD(IsPackageSetReady_N_No_NotAllPackageStatusOK)
        {
            BEGIN_TEST_METHOD_PROPERTIES()
                TEST_METHOD_PROPERTY(L"RunAs", L"ElevatedUser")
            END_TEST_METHOD_PROPERTIES()

            auto packageDeploymentManager{ winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentManager::GetDefault() };

            AddPackage_Red();
            AddPackage_Green();
            SetPackageStatusByPackageFamilyName(::TPF::Green::c_packageFamilyName, winrt::Windows::Management::Deployment::PackageStatus::Modified);
            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageReady(::TPF::Green::GetPackageFullName()));

            winrt::Microsoft::Windows::Management::Deployment::PackageSet packageSet;
            PCWSTR c_packageSetId{ L"RGB" };
            packageSet.Id(c_packageSetId);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem red{ Make_PackageSetItem(::TPF::Red::GetPackageFullName(), ::TPF::Red::c_packageDirName) };
            packageSet.Items().Append(red);
            winrt::Microsoft::Windows::Management::Deployment::PackageSetItem green{ Make_PackageSetItem(::TPF::Green::GetPackageFullName(), ::TPF::Green::c_packageDirName) };
            packageSet.Items().Append(green);

            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageSetReady(packageSet));
            VERIFY_IS_FALSE(packageDeploymentManager.IsPackageReady(::TPF::Green::GetPackageFullName()));

            ClearPackageStatusByPackageFamilyName(::TPF::Green::c_packageFamilyName, winrt::Windows::Management::Deployment::PackageStatus::Modified);
        }
    };
}
