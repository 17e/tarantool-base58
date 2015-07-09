Name:    tarantool-base58
Version: 0.01	
Release: 1%{?dist}
Summary: base58 encode/decode subroutines for tarantool 1.5	

Group:	 Development/Libraries
License: GPLv2	
URL:		 https://github.com/17e/tarantool-base58
Source0: tarantool-base58.tar.bz2

BuildRequires:	tarantool-dev < 1.6
BuildRequires:	autoconf
Requires:	tarantool < 1.6

%description
https://github.com/17e/tarantool-base58


%prep
%setup -q -n tarantool-base58


%build
autoconf configure.ac > configure
chmod +x configure
%configure
make %{?_smp_mflags}


%install
make install DESTDIR=%{buildroot}


%files
%defattr(-,root,root,-)
%doc README
%attr(0755, root, root) %{_libdir}/tarantool/base58.so


%changelog
* Thu Jul 09 2015 Maxim Polyakov 0.01-1
- Create spec file

